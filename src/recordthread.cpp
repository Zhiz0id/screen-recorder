#include "recordthread.h"

using namespace std;

//extern "C" int cmain(int argc, char **argv);

RecordThread::RecordThread(QString threadName) :
    name(threadName)
{
}

void RecordThread::run()
{
    qDebug() << "start RecordThread";
    char* strs[] = {"test\0", "-o\0", "/home/defaultuser/Videos/smile.ogv\0", "127.0.0.1:5900\0"};
    char** argv = strs;
    cmain(4,argv);
}
