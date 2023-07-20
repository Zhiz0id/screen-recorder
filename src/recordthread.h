#ifndef RECORDTHREAD_H
#define RECORDTHREAD_H
 
#include <QThread>
#include <QDebug>
#include "vnc2mpg.h"
#include <QStandardPaths>
#include <QDateTime>
#include <ctime>
#include <cstdlib>
class RecordThread : public QThread
{
Q_OBJECT
public:
    explicit RecordThread(QString threadName);
 
    int stop;
    void stopit();
private:
    QString name;   
    void run();
};
 
#endif // RECORDTHREAD_H
