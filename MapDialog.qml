import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: root
    title: "Map Directories"
    width: 500
    height: 800

    property var tempMapPaths: []

    function resetTempPaths() {
        tempMapPaths = userSettings.mapPaths.slice()
        pathList.model = tempMapPaths
    }

    Component.onCompleted: {
        resetTempPaths()
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        ListView {
            id: pathList
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: tempMapPaths
            delegate: ItemDelegate {
                width: parent.width
                contentItem: RowLayout {
                    Label {
                        text: modelData
                        wrapMode: Text.Wrap
                        Layout.fillWidth: true
                    }
                    Button {
                        text: "Delete"
                        onClicked: {
                            tempMapPaths.splice(index, 1)
                            pathList.model = tempMapPaths
                        }
                    }
                }
            }
        }

        Button {
            text: "Add Directory"
            Layout.alignment: Qt.AlignRight
            onClicked: {
                var newPath = userSettings.addMapPath()
                if (newPath && !tempMapPaths.includes(newPath)) {
                    tempMapPaths.push(newPath)
                    pathList.model = tempMapPaths
                }
            }
        }
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: {
        userSettings.setMapPaths(tempMapPaths)
    }

    onRejected: {
        resetTempPaths()
    }

    onAboutToShow: {
        resetTempPaths()
    }
}
