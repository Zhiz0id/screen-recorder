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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QString>
#include <QtCore>
#include <QDebug>
#include <QStandardPaths>


class Settings : public QObject
{
    Q_OBJECT
//    Q_ENUMS(Codec)
    Q_PROPERTY(int scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(int fps READ fps WRITE setFps NOTIFY fpsChanged)
    Q_PROPERTY(int bps READ bps WRITE setBps NOTIFY bpsChanged)
//    Q_PROPERTY(Settings::Codec codec READ codec WRITE setCodec NOTIFY codecChanged)

public:
/*
    enum Codec
    {
        MPEG2,
        AVI,
        H264,
        H265,
        Theora
    };
*/
    explicit Settings(const QString &settingsPath = 
        QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation).append("/settings.ini"), 
        QObject *parent = nullptr);

    int scale() const;
    void setScale(const int &scale);

    int fps() const;
    void setFps(const int &fps);

    int bps() const;
    void setBps(const int &bps);

//    Settings::Codec codec() const;
//    void setCodec(const Codec &codec);

private:
    void setValue(const QString &key, const QVariant &value = QVariant());
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;

signals:
    void scaleChanged(int scale);
    void fpsChanged(int fps);
    void bpsChanged(int bps);
    void codecChanged();

private:
    QSettings *m_settings;
};

#endif // SETTINGS_H
