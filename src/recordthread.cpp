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

#include "recordthread.h"

#define SCALE   QStringLiteral("screenrecorder/scale")
#define FPS     QStringLiteral("screenrecorder/fps")
#define BPS     QStringLiteral("screenrecorder/bps")
#define CODEC   QStringLiteral("screenrecorder/codecindex")

using namespace std;

//extern "C" int cmain(int argc, char **argv);

RecordThread::RecordThread(QString threadName) :
    name(threadName), m_settings(nullptr)
{
    /*
    const QString settingsPath = 
        QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)
        .append("/settings.ini");

    if (settingsPath.isEmpty()) {
        settings = new QSettings(this);
    } else {
        QSettings settings(settingsPath, QSettings::IniFormat, this);
    }
    */
}

void RecordThread::run()
{
    QString dateString = QDateTime::currentDateTime().toString(QStringLiteral("yy-MM-dd_HH-mm-ss"));
    QString filename = QStringLiteral("/sr-%1.mp4").arg(dateString);

    QString filepath = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + filename;
    Settings *m_settings = new Settings();
    int scale = m_settings->scale();
    int fps = m_settings->fps();
    int bps = m_settings->bps();
    //int codec = static_cast<RecordThread::Codec>(settings.value(CODEC, MPEG2).toInt());

    stop = 0;
    cmain(filepath.toStdString().data(), &stop, &scale, &fps, &bps);
}
void RecordThread::stopit()
{
    //std::srand(std::time(nullptr));
    //int random_variable = std::rand();
    stop = 1;//random_variable;
}
