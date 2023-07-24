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

#include <auroraapp.h>
#include <QtQuick>

#include "settings.h"
#include "interfacer.h"


int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> application(Aurora::Application::application(argc, argv));
    application->setOrganizationName(QStringLiteral("info.you_ra"));
    application->setApplicationName(QStringLiteral("screen_recorder"));

    QScopedPointer<QQuickView> view(Aurora::Application::createView());
    
    // Add Qt translations
    QTranslator *translator = new QTranslator();
    translator->load(QLocale::system(), "screen_recorder", "_", Aurora::Application::pathTo("translations").toLocalFile(), ".qm");
    application->installTranslator(translator);

    auto context = view->rootContext();
    qmlRegisterType<Interfacer>("info.you_ra.ScreenRecorder", 1, 0, "Interfacer");
    qmlRegisterType<Settings>("info.you_ra.ScreenRecorder", 1, 0, "Settings");

    Interfacer interfacer;
    context->setContextProperty("srec", &interfacer);

    view->setSource(Aurora::Application::pathTo(QStringLiteral("qml/screen_recorder.qml")));
    view->show();

    return application->exec();
}

