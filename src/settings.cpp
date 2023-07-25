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

#include "settings.h"

#define SCALE   QStringLiteral("screenrecorder/scale")
#define FPS     QStringLiteral("screenrecorder/fps")
#define BPS     QStringLiteral("screenrecorder/bps")
#define CODEC   QStringLiteral("screenrecorder/codecindex")

Settings::Settings(const QString &settingsPath, QObject *parent) : QObject(parent)
{
    if (settingsPath.isEmpty()) {
        m_settings = new QSettings(this);
    } else {
        m_settings = new QSettings(settingsPath, QSettings::IniFormat, this);
    }
}

// scale
int Settings::scale() const
{
    return value(SCALE, 2).toInt();
}

void Settings::setScale(const int &scale)
{
    setValue(SCALE, scale);
    emit this->scaleChanged(scale);
}

// fps
int Settings::fps() const
{
    return value(FPS, 25).toInt();
}

void Settings::setFps(const int &fps)
{
    setValue(FPS, fps);
    emit this->fpsChanged(fps);
}

// bps
int Settings::bps() const
{
    return value(BPS, 2048).toInt();
}

void Settings::setBps(const int &bps)
{
    setValue(BPS, bps);
    emit this->bpsChanged(bps);
}

// codec
//Settings::Codec Settings::codec() const
//{
//    return static_cast<Settings::Codec>(settings.value(CODEC, MPEG2).toInt());
//}

//void Settings::setCodec(const Codec &codec)
//{
//    if(this->codec() != codec){
//        settings.setValue(CODEC, codec);
//        emit this->codecChanged();
//    }
//}

void Settings::setValue(const QString &key, const QVariant &value)
{
    m_settings->setValue(key, value);
    m_settings->sync();
}

QVariant Settings::value(const QString &key, const QVariant &defaultValue) const
{
    m_settings->sync();
    return m_settings->value(key, defaultValue);
}

