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

#ifndef RECORDTHREAD_H
#define RECORDTHREAD_H
 
#include <QThread>
#include <QDebug>
#include "vnc2mpg.h"
#include <QStandardPaths>
#include <QDateTime>
#include <QSettings>
#include <ctime>
#include <cstdlib>
#include "settings.h"

class RecordThread : public QThread
{
Q_OBJECT
public:
    explicit RecordThread(QString threadName);
 
    int stop;
    void stopit();

private:
    QString name;   
    Settings *m_settings;
    void run();
};
 
#endif // RECORDTHREAD_H
