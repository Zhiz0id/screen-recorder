#include "interfacer.h"


#define SCALE   QStringLiteral("screenrecorder/scale")
#define FPS     QStringLiteral("screenrecorder/fps")
#define BPS     QStringLiteral("screenrecorder/bps")
#define CODEC   QStringLiteral("screenrecorder/codecindex")

Interfacer::Interfacer()//: recordT(new RecordThread("A"))
{
    running = false;
    //connect(this, &Interfacer::state, this, &Interfacer::stateChanged);
}

Interfacer::State Interfacer::state() const
{
    if (running) {
        return Interfacer::RecordingState;
    }
    return Interfacer::StoppedState;
}

void Interfacer::setState(const State &state)
{
    if(this->state() != state){
        state == Interfacer::RecordingState ? running = true : running = false;
        emit this->stateChanged(state);
    }
}


Interfacer::Status Interfacer::status() const
{
    if (recordT->isRunning()) {
        return RecordingStatus; 
    }

    return UnavailableStatus;
}

void Interfacer::setStatus(const Status &status)
{
    if(this->status() != status){
        emit this->statusChanged(status);
    }
}


void Interfacer::startRecording()
{
    //RecordThread* recordT = new RecordThread("A");
    //this->recordT("A");
    qDebug() << "pre Interfacer::start()";
    if (this->state() != StoppedState) {
        return;
    }

    recordT = new RecordThread("A");
    recordT->start();
    qDebug() << "Interfacer::start()";
    connect(recordT, &RecordThread::started, this, &Interfacer::onStartedRecording);
    connect(recordT, &RecordThread::finished, this, &Interfacer::onFinishedRecording);
    this->setState(Interfacer::RecordingState);
}

void Interfacer::onStartedRecording()
{
    qDebug() << "Interfacer::onStartedRecording event";
}

void Interfacer::onFinishedRecording()
{
    qDebug() << "Interfacer::onFinishedRecording event";
}

void Interfacer::stopRecording()
{
    if(this->state() != RecordingState){
        return;
    }

    recordT->stopit();
    recordT->wait();
    recordT = NULL;
    //recordT->terminate();
    this->setState(Interfacer::StoppedState);
}
// scale
int Interfacer::scale() const
{
    return settings.value(SCALE, 1).toInt();
}

void Interfacer::setScale(const int &scale)
{
    if(this->scale() != scale){
        settings.setValue(SCALE, scale);
        emit this->scaleChanged(scale);
    }
}

// fps
int Interfacer::fps() const
{
    return settings.value(FPS, 25).toInt();
}

void Interfacer::setFps(const int &fps)
{
    if(this->fps() != fps){
        settings.setValue(FPS, fps);
        emit this->fpsChanged(fps);
    }
}

// bps
int Interfacer::bps() const
{
    return settings.value(BPS, 2048).toInt();
}

void Interfacer::setBps(const int &bps)
{
    if(this->bps() != bps){
        settings.setValue(BPS, bps);
        emit this->bpsChanged(bps);
    }
}

// codec
Interfacer::Codec Interfacer::codec() const
{
    return static_cast<Interfacer::Codec>(settings.value(CODEC, MPEG2).toInt());
}

void Interfacer::setCodec(const Codec &codec)
{
    if(this->codec() != codec){
        settings.setValue(CODEC, codec);
        emit this->codecChanged();
    }
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
