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

#ifndef INTERFACER_H
#define INTERFACER_H

#include <QObject>
#include <QString>
#include <QtCore>
#include <QDebug>
#include <QStandardPaths>
#include <QSettings>
#include "recordthread.h"
class Interfacer : public QObject
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_ENUMS(Status)
    Q_ENUMS(Error)
    Q_ENUMS(Codec)
    Q_PROPERTY(Interfacer::State state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(Interfacer::Status status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(int scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(int fps READ fps WRITE setFps NOTIFY fpsChanged)
    Q_PROPERTY(int bps READ bps WRITE setBps NOTIFY bpsChanged)
    Q_PROPERTY(Interfacer::Codec codec READ codec WRITE setCodec NOTIFY codecChanged)
public:
    Interfacer();

    enum State
    {
        StoppedState,
        RecordingState
    };

    enum Status {
        UnavailableStatus,
        UnloadedStatus,
        LoadingStatus,
        LoadedStatus,
        StartingStatus,
        RecordingStatus,
        FinalizingStatus
    };

    enum Error
    {
        NoError,
        ResourceError,
        FormatError,
        OutOfSpaceError
    };

    enum Codec
    {
        MPEG2,
        AVI,
        H264,
        H265,
        Theora
    };

    int scale() const;
    void setScale(const int &scale);
    int fps() const;
    void setFps(const int &fps);
    int bps() const;
    void setBps(const int &bps);
    Interfacer::State state() const;
    void setState(const State &state);
    Interfacer::Status status() const;
    void setStatus(const Status &status);
    Interfacer::Codec codec() const;
    void setCodec(const Codec &codec);
    bool running;
public Q_SLOTS:
//    void setRunning(bool running);
    void startRecording();
    void stopRecording();
    bool isFinished();
    bool isRunning();

Q_SIGNALS:
//    void runningChanged(bool running);
    void stateChanged(Interfacer::State state);
    void statusChanged(Interfacer::Status status);
    void scaleChanged(int scale);
    void fpsChanged(int fps);
    void bpsChanged(int bps);
    void codecChanged();

private:
    RecordThread *recordT;
    QSettings settings;


private slots:
    void onStartedRecording();
    void onFinishedRecording();
};

#endif // INTERFACER_H
