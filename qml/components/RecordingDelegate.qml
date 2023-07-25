import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Share 1.0

ListItem {
    menu: ContextMenu {

        MenuItem {
            text: qsTr("Share")
            onClicked: {
                shareAction.resources = [filePath]
                shareAction.trigger()
            }
            ShareAction {
                id: shareAction
                mimeType: "video/mp4" //fileType
            }
        }


        MenuItem {
            text: qsTr("Rename")
            onClicked: pageStack.push(Qt.resolvedUrl("../pages/RenameDialog.qml"), {
                                          fileName: fileName,
                                          fileDir: fileDir
                                      })
        }

        MenuItem {
            text: qsTr("Delete")
            onClicked: {
                remorseAction(qsTr("Deleting"), function() {
                    srec.removeFile(filePath)
                })
            }
        }
    }

    ListView.onRemove: animateRemoval()

    onClicked: pageStack.push(Qt.resolvedUrl("../pages/ViewVideoPage.qml"), { path: filePath, title: fileName, autoPlay: true });

    Label {
        id: filenameLabel
        anchors {
            left: parent.left
            right: parent.right
            leftMargin: Theme.horizontalPageMargin
            rightMargin: Theme.horizontalPageMargin
        }
        text: fileName
        anchors.verticalCenter: parent.verticalCenter
        color: parent.highlighted ? Theme.highlightColor : Theme.primaryColor
    }
}
