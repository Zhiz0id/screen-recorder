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

Page {
    id: page
    allowedOrientations: Orientation.All

    //Component.onCompleted: settingsPage = this

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Settings")
            }

            SectionHeader {
                text: qsTr("Scale")
            }

            Slider {
                width: parent.width
                minimumValue: 1.0
                maximumValue: 4.0
                stepSize: 1.0
                valueText: qsTr("1:%1").arg(value).toLocaleString()
                label: qsTr("Scale of video")

                onValueChanged: srec.scale = value

                Component.onCompleted: value = srec.scale
            }

            SectionHeader {
                text: qsTr("Frame per second")
            }

            Slider {
                width: parent.width
                minimumValue: 1.0
                maximumValue: 60.0
                stepSize: 1.0
                valueText: qsTr("%1fps").arg(value).toLocaleString()
                label: qsTr("fps")

                onValueChanged: srec.fps = value

                Component.onCompleted: value = srec.fps
            }

            SectionHeader {
                text: qsTr("Bitrate")
            }

            Slider {
                width: parent.width
                minimumValue: 32.0
                maximumValue: 4096.0
                stepSize: 32.0
                valueText: qsTr("%1K bps").arg(value).toLocaleString()
                label: qsTr("bps")

                onValueChanged: srec.bps = value

                Component.onCompleted: value = srec.bps
            }


            SectionHeader {
                text: qsTr("Codec")
            }

            ComboBox {
                width: parent.width
                label: qsTr("Codec")
                currentIndex: srec.codec
                description: qsTr("Some codecs might be not supported by some players.")

                onCurrentItemChanged: srec.codec = currentIndex

                menu: ContextMenu {
                    MenuItem { text: "MPEG-2" }
                    MenuItem { text: "AVI" }
                    MenuItem { text: "H.264" }
                    MenuItem { text: "H.265" }
                    MenuItem { text: "Theora" }
                }
            }
        }
    }
}
