#include "recordthread.h"

using namespace std;

//extern "C" int cmain(int argc, char **argv);

RecordThread::RecordThread(QString threadName) :
    name(threadName)
{
}

void RecordThread::run()
{
    QString dateString = QDateTime::currentDateTime().toString(QStringLiteral("dd-MM-yy_HH-mm-ss"));
    QString filename = QStringLiteral("/sr-%1.mp4").arg(dateString);

    //qDebug() << QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + filename << "!!!!!!!!!!!!!!!!!!!!!!!!!!";
    QString filepath = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + filename;
    //char *c_filepath = filepath.toLocal8Bit().data();
    qDebug() << "start RecordThread";
    qDebug() << filepath;
    //std::vector<std::string> arguments = {"test", "-o", filepath.toStdString().data(), "127.0.0.1:5900"};
    //for (const auto& x : arguments){
    //    qDebug() << x.data();
    //}
    //std::vector<char*> argv;
    //for (const auto& arg : arguments) {
    //    argv.push_back((char*)arg.data());
    //}
    //argv.push_back(nullptr);

    //char* strs[] = {"test\0", "-o\0", strdup(filepath.toStdString().data()), "127.0.0.1:5900\0"};
    //char** argv = strs;
    stop = 0;
    cmain(filepath.toStdString().data(), &stop);
}
void RecordThread::stopit()
{
    //std::srand(std::time(nullptr));
    //int random_variable = std::rand();
    stop = 1;//random_variable;
}
