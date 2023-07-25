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
import info.you_ra.ScreenRecorder 1.0
import "../components"

Page {
    objectName: "mainPage"
    allowedOrientations: Orientation.All

    SilicaListView {
        id: recordingsList

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: control.top
            bottomMargin: Theme.paddingLarge
        }

        enabled: opacity > 0.0
        currentIndex: -1
        clip: true
        model: recordingsModel
        header: PageHeader {
                objectName: "pageHeader"
                title: qsTr("Screen recorder")
                description: qsTr("Record button - to record your screen. Settings in pulley menu to change properties.")
                extraContent.children: [
                    IconButton {
                        objectName: "aboutButton"
                        icon.source: "image://theme/icon-m-about"
                        anchors.verticalCenter: parent.verticalCenter

                        onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
                    }
                ]
            }

        delegate: RecordingDelegate { }

        section {
            property: "section"
            delegate: SectionHeader {
                text: section
            }
        }
        PullDownMenu {
            MenuItem {
                text: qsTr("Settings")
               onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"))
            }
        }

        ViewPlaceholder {
            enabled: !recordingsList.count
            text: qsTr("Your recordings will be shown here")
        }

        VerticalScrollDecorator { }
    }

    Item {
        id: control
        width: parent.width
        //anchors.fill: parent
        anchors {
            left: parent.left
            right: parent.right
	    bottom: parent.bottom
            margins: Theme.paddingLarge
        }
        height: buttons.height + Theme.paddingLarge * 2

        Row{
            id: buttons
            width: parent.width
            spacing: Theme.paddingLarge


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
        }
    }
}
