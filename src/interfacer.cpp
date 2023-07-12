#include "interfacer.h"

Interfacer::Interfacer()
{
}

void Interfacer::start()
{
    RecordThread* recordT = new RecordThread("A");
    recordT->start();
    qDebug() << "Interfacer::start()";
}


