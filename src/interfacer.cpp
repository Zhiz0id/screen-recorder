/*******************************************************************************
**
** Copyright (C) 2023 Yura Beznos at You-ra.info
**
** This file is part of the Screen recorder application project.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write an email to the license@you-ra.info
*******************************************************************************/

#include "interfacer.h"


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
