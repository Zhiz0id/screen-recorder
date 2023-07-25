#include "recordingsmodel.h"
#include "interfacer.h"
#include <QFileSystemWatcher>
#include <QDirIterator>
#include <QDateTime>

const QStringList RecordingsModel::filters{
    "*.ogv",
    "*.mp4",
    "*.avi"
};

RecordingsModel::RecordingsModel(QObject *parent) :
    QAbstractListModel(parent),
    mInterfacer(0),
    mWatcher(new QFileSystemWatcher(this))
{
    connect(mWatcher, &QFileSystemWatcher::directoryChanged, this, &RecordingsModel::scanRecords);
}

Interfacer *RecordingsModel::interfacer() const
{
    return mInterfacer;
}

void RecordingsModel::setInterfacer(Interfacer *interfacer)
{
    if (mInterfacer == interfacer)
    {
        return;
    }

    if (mInterfacer)
    {
        //disconnect(mInterfacer, &Interfacer::locationChanged, this, &RecordingsModel::resetModel);
        //disconnect(mInterfacer, &Interfacer::recursiveSearchChanged, this, &RecordingsModel::resetModel);
    }

    mInterfacer = interfacer;
    //connect(mInterfacer, &Interfacer::locationChanged, this, &RecordingsModel::resetModel);
    //connect(mInterfacer, &Interfacer::recursiveSearchChanged, this, &RecordingsModel::resetModel);

    this->resetModel();
}

bool RecordingsModel::contains(const QString &filePath) const
{
    return mData.contains(QFileInfo(filePath));
}

void RecordingsModel::scanRecords(const QString &path)
{
    Q_ASSERT(mInterfacer);

    // Current watching directories
    auto currentDirs = mWatcher->directories();

    // Is the recursive search enabled
    //auto recursiveSearch = mInterfacer->recursiveSearch();

    // Check if directory exists and add or remove it and
    // its subdirectories to the wathing list
    qDebug() << path;
    QDir dir(path);
    QStringList paths(path);
    if (dir.exists())
    {
        // Check for subdirectories that should be added
        //if (recursiveSearch)
        //{
        //    QDirIterator it(path, QDir::Dirs, QDirIterator::Subdirectories);
        //    while (it.hasNext())
        //    {
        //        paths << it.next();
        //    }
        //}
        mWatcher->addPaths(paths);
    }
    else
    {
        // Check for subdirectories that should be removed
        for (auto d: currentDirs)
        {
            if (d.startsWith(path))
            {
                paths << d;
            }
        }
        mWatcher->removePaths(paths);
    }

    // Scan for updated and removed records
    for (int row = mData.size() - 1; row > -1; --row)
    {
        auto oldFileInfo = mData[row];
        // Process entries only of the changed path
        if (oldFileInfo.absolutePath() != path)
        {
            continue;
        }
        QFileInfo newFileInfo(oldFileInfo);
        newFileInfo.refresh();
        if (newFileInfo.exists())
        {
            if (newFileInfo.lastModified() != oldFileInfo.lastModified())
            {
                // The record was updated
                auto index = this->createIndex(row, 0);
                emit this->dataChanged(index, index, { Modified });
            }
        }
        else
        {
            // The record was removed
            this->beginRemoveRows(QModelIndex(), row, row);
            mData.removeAt(row);
            this->endRemoveRows();
        }
    }

    // Scan for new records
    //auto flags = recursiveSearch ? QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags;
    auto flags = QDirIterator::NoIteratorFlags;
    QDirIterator it(path, RecordingsModel::filters, QDir::Files, flags);
    QFileInfoList newRecords;
    while (it.hasNext())
    {
        // Add a new record
        QFileInfo fileInfo(it.next());
        if (!mData.contains(fileInfo))
        {
            newRecords << fileInfo;
        }
    }
    if (!newRecords.empty())
    {
        auto pos = mData.size();
        this->beginInsertRows(QModelIndex(), pos, pos + newRecords.size() - 1);
        mData.append(newRecords);
        this->endInsertRows();
    }
}

void RecordingsModel::resetModel()
{
    this->beginResetModel();
    auto currentDirs = mWatcher->directories();
    if (!currentDirs.isEmpty())
    {
        mWatcher->removePaths(currentDirs);
    }
    mData.clear();
    this->endResetModel();
    if (mInterfacer)
    {
        this->scanRecords(mInterfacer->location());
    }
}

inline QString RecordingsModel::sectionName(const QDate &modDate)
{
    auto curDate = QDate::currentDate();
    auto days = modDate.daysTo(curDate);
    if (days == 0)
    {
        return tr("Today");
    }
    if (days == 1)
    {
        return tr("Yesterday");
    }
    if (days < 7 && modDate.dayOfWeek() < curDate.dayOfWeek())
    {
        return tr("This week");
    }
    if (days < curDate.daysInMonth() && modDate.day() < curDate.day())
    {
        return tr("This month");
    }
    if (days < 183)
    {
        return tr("Last 6 months");
    }
    return tr("Older");
}

int RecordingsModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mData.size();
}

QVariant RecordingsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    auto fileInfo = mData[index.row()];

    switch (role)
    {
    case FilePath:
        return fileInfo.absoluteFilePath();
    case FileName:
        return fileInfo.fileName();
    case FileDir:
        return fileInfo.absolutePath();
    case Modified:
        return fileInfo.lastModified();
    case Section:
        return RecordingsModel::sectionName(fileInfo.lastModified().date());
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> RecordingsModel::roleNames() const
{
    return {
        { FilePath, "filePath" },
        { FileName, "fileName" },
        { FileDir,  "fileDir"  },
        { Modified, "modified" },
        { Section,  "section"  }
    };
}
