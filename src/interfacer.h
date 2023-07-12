#ifndef INTERFACER_H
#define INTERFACER_H

#include <QObject>
#include <QString>
#include <QtCore>
#include <QDebug>
#include "recordthread.h"
class Interfacer : public QObject
{
    Q_OBJECT

public:
    Interfacer();

private:
    RecordThread *recordT;
    //bool running;

signals:

//    void runningChanged(bool running);

public slots:
//    void setRunning(bool running);
    void start();


private slots:
//    void onConnected();
//    void onDisconnected();
};

#endif // INTERFACER_H
