#include "interfacer.h"

Interfacer::Interfacer()//: recordT(new RecordThread("A"))
{
}

void Interfacer::start()
{
    //RecordThread* recordT = new RecordThread("A");
    //this->recordT("A");
    if(!recordT) {
        recordT = new RecordThread("A");
        recordT->start();
        qDebug() << "Interfacer::start()";
    }
        qDebug() << "already started Interfacer::start()";

}

void Interfacer::quit()
{
    if(recordT) {
        recordT->stopit();
        recordT->wait();
        recordT = NULL;
        //recordT->terminate();
        qDebug() << "Interfacer::quit()";
    }
    qDebug() << "empty Interfacer::quit()";

}

bool Interfacer::isFinished()
{
    if(recordT) {
        return recordT->isFinished();
    } else {
        return true;
    }
}

bool Interfacer::isRunning()
{
    if(recordT) {
        return recordT->isRunning();
    } else {
        return false;
    }
}
