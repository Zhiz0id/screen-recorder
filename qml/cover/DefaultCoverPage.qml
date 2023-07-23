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

CoverBackground {

  Column {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        spacing: Theme.paddingLarge
        Column {
            id: reclogo
            width: parent.width
            height: parent.height / 2
            HighlightImage {
                id: logo
                width: parent.width
                height: parent.height
                //anchors.CenterIn: parent
                source: "../icons/screen_recorder.svg"
                fillMode: Image.PreserveAspectFit
            }
        }

        Label {
            width: parent.width
            font.pixelSize: Theme.fontSizeLarge
            text: qsTr("Screen Recorder")
            horizontalAlignment: Qt.AlignHCenter
        }
        Label {
            width: parent.width
            font.pixelSize: Theme.fontSizeLarge
            text: {
                switch (srec.state) {
                case Interfacer.StoppedState:
                    return qsTr("stopped")
                case Interfacer.RecordingState:
                    return qsTr("recording...")
                default:
                    return ""
                }
            }
            horizontalAlignment: Qt.AlignHCenter
        }
    }


    CoverActionList {
            CoverAction {
                iconSource: srec.state == Interfacer.RecordingState ?
                    "image://theme/icon-cover-cancel" : "image://theme/icon-cover-record"
                onTriggered: srec.state == Interfacer.RecordingState ?
                    srec.stopRecording() : srec.startRecording()
            }
    }
}
