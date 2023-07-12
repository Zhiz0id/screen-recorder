#ifndef RECORDTHREAD_H
#define RECORDTHREAD_H
 
#include <QThread>
#include <QDebug>
#include "vnc2mpg.h"

class RecordThread : public QThread
{
public:
    explicit RecordThread(QString threadName);
 
    void run();
private:
    QString name;   
};
 
#endif // RECORDTHREAD_H
