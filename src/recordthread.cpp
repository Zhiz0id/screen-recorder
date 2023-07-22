#include "recordthread.h"

#define SCALE   QStringLiteral("screenrecorder/scale")
#define FPS     QStringLiteral("screenrecorder/fps")
#define BPS     QStringLiteral("screenrecorder/bps")
#define CODEC   QStringLiteral("screenrecorder/codecindex")

using namespace std;

//extern "C" int cmain(int argc, char **argv);

RecordThread::RecordThread(QString threadName) :
    name(threadName)
{
}

void RecordThread::run()
{
    QString dateString = QDateTime::currentDateTime().toString(QStringLiteral("yy-MM-dd_HH-mm-ss"));
    QString filename = QStringLiteral("/sr-%1.mp4").arg(dateString);

    QString filepath = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + filename;
    qDebug() << "start RecordThread";
    qDebug() << filepath;
    int scale = settings.value(SCALE, 1).toInt();
    int fps = settings.value(FPS, 25).toInt();
    int bps = settings.value(BPS, 2048).toInt();
    int codec = static_cast<RecordThread::Codec>(settings.value(CODEC, MPEG2).toInt());

    stop = 0;
    cmain(filepath.toStdString().data(), &stop, &scale, &fps, &bps, &codec);
}
void RecordThread::stopit()
{
    //std::srand(std::time(nullptr));
    //int random_variable = std::rand();
    stop = 1;//random_variable;
}
