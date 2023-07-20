/*******************************************************************************
**
** Copyright (C) 2022 info.you_ra
**
** This file is part of the Screen recorder application project.
**
** Redistribution and use in source and binary forms,
** with or without modification, are permitted provided
** that the following conditions are met:
**
** * Redistributions of source code must retain the above copyright notice,
**   this list of conditions and the following disclaimer.
** * Redistributions in binary form must reproduce the above copyright notice,
**   this list of conditions and the following disclaimer
**   in the documentation and/or other materials provided with the distribution.
** * Neither the name of the copyright holder nor the names of its contributors
**   may be used to endorse or promote products derived from this software
**   without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
** THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
** FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
** OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
** PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS;
** OR BUSINESS INTERRUPTION)
** HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
** WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE)
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
** EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
*******************************************************************************/

import QtQuick 2.0
import Sailfish.Silica 1.0
import ScreenRecorder.Interfacer 1.0

Page {
    objectName: "mainPage"
    allowedOrientations: Orientation.All

    Interfacer {
        id: screenrecorderInterface
        //running: Qt.application.state === Qt.ApplicationActive
    }
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
              text: qsTr("Recording")
            }

            IconButton {
                id: startButton
                icon.source: "../icons/record.png"
                enabled: true //screenrecorderInterface.isFinished()
                onClicked: {
                    console.log("start");
                    screenrecorderInterface.start();
                }

            }
            IconButton {
                id: stopButton
                icon.source: "../icons/stop.png"
                enabled: true //screenrecorderInterface.isRunning()
                onClicked: {
                    console.log("quit");
                    screenrecorderInterface.quit();
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
