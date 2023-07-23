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

import QtQuick 2.0
import Sailfish.Silica 1.0
import ScreenRecorder.Interfacer 1.0

Page {
    objectName: "mainPage"
    allowedOrientations: Orientation.All

    SilicaFlickable{
        //id: dockTools
        //width: parent.width
        anchors.fill: parent
        contentHeight: column.height
        //height: 100
        //open: true
        //dock: Dock.Bottom

            Column{
            id: column
            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                objectName: "pageHeader"
                title: qsTr("Screen recorder")
                extraContent.children: [
                    IconButton {
                        objectName: "aboutButton"
                        icon.source: "image://theme/icon-m-about"
                        anchors.verticalCenter: parent.verticalCenter
        
                        onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
                    }
                ]
            }

            SectionHeader {
              text: qsTr("Control")
            }

            ButtonLayout {
                Button {
                    text: {
                        switch (srec.state) {
                            case Interfacer.StoppedState:
                                return qsTr("Record")
                            case Interfacer.RecordingState:
                                return qsTr("Stop")
                            default:
                                return ""
                        }
                    }
                    onClicked: {
                        switch (srec.state) {
                            case Interfacer.StoppedState:
                                srec.startRecording();
                                return;
                            case Interfacer.RecordingState:
                                srec.stopRecording();
                                return;
                            default:
                                console.log("default action?");
                                return;
                        }
                    }
                }
            }
            PullDownMenu {
                MenuItem {
                    text: qsTr("Settings")
                    onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"))
                }
            }

        }
    }
}
