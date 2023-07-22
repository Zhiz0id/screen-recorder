#ifndef RECORDTHREAD_H
#define RECORDTHREAD_H
 
#include <QThread>
#include <QDebug>
#include "vnc2mpg.h"
#include <QStandardPaths>
#include <QDateTime>
#include <QSettings>
#include <ctime>
#include <cstdlib>
class RecordThread : public QThread
{
Q_OBJECT
Q_ENUMS(Codec)
public:
    explicit RecordThread(QString threadName);
 
    int stop;
    void stopit();

    enum Codec
    {
        MPEG2,
        AVI,
        H264,
        H265,
        Theora
    };

private:
    QString name;   
    QSettings settings;
    void run();
};
 
#endif // RECORDTHREAD_H
