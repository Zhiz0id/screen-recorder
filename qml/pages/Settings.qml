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
                minimumValue: 1.0
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
/*
            ComboBox {
                width: parent.width
                label: qsTr("Encoding Quality")
                currentIndex: recorder.encodingQuality

                onCurrentItemChanged: recorder.encodingQuality = currentIndex

                menu: ContextMenu {
                    MenuItem { text: qsTr("Very low") }
                    MenuItem { text: qsTr("Low") }
                    MenuItem { text: qsTr("Normal") }
                    MenuItem { text: qsTr("High") }
                    MenuItem { text: qsTr("Very high") }
                }
            }

            ComboBox {
                width: parent.width
                label: qsTr("Encoding Mode")
                description: {
                    switch (currentIndex) {
                        case 0:  return qsTr("Constant quality with adjusted bit rate to fit.")
                        case 1:  return qsTr("Constant bit rate with adjusted quality to fit.")
                        case 2:  return qsTr("Variable bit rate with fixed average value.")
                        case 3:  return qsTr("Process the recording at first to determine its characteristics, \
and then process a second time to allocate more bits to the areas that need it.")
                        default: return ""
                    }
                }

                currentIndex: recorder.encodingMode

                onCurrentItemChanged: recorder.encodingMode = currentIndex

                menu: ContextMenu {
                    MenuItem { text: qsTr("Constant quality") }
                    MenuItem { text: qsTr("Constant bit rate") }
                    MenuItem { text: qsTr("Average bit rate") }
                    MenuItem { text: qsTr("Two pass encoding") }
                }
            }

            ComboBox {
                property variant rates: [
                    0,
                    8000,
                    11025,
                    16000,
                    22050,
                    32000,
                    44100
                ]

                width: parent.width
                label: qsTr("Sample Rate")
                currentIndex: rates.indexOf(recorder.sampleRate)

                onCurrentItemChanged: recorder.sampleRate = rates[currentIndex]

                menu: ContextMenu {
                    MenuItem { text: qsTr("auto") }
                    MenuItem { text: qsTr("%0kHz").arg(8) }
                    MenuItem { text: qsTr("%0kHz").arg(11) }
                    MenuItem { text: qsTr("%0kHz").arg(16) }
                    MenuItem { text: qsTr("%0kHz").arg(22) }
                    MenuItem { text: qsTr("%0kHz").arg(32) }
                    MenuItem { text: qsTr("%0kHz").arg(44) }
                }
            }
*/
        }
    }
}
