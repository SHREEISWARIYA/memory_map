// RightDock.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Drawer {
    id: infoPanel
    width: 250
    //height: parent.height
    height: parent.height - bottomBar.height
    edge: Qt.RightEdge
    modal: false
    interactive: false
    opacity: 0.9
    visible: false

    property var currentShip: null
    property var mapView: null
    property var pastTrail: null
    property string currentShipUuid: ""

    signal fetchTrackHistoryRequested(string uuid)
    signal shipDetailsRequested(string name, real latitude, real longitude)
    signal hidePastTrackRequested()

    background: Rectangle {
        color: "#ffffff"
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Top bar
        Rectangle {
            Layout.fillWidth: true
            height: 70
            color: "#2c3e50"

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10

                Text {
                    Layout.fillWidth: true
                    text: currentShip ? currentShip.track_name.trim() : "Ship Information"
                    font.pixelSize: 14
                    font.bold: true
                    color: "white"
                    elide: Text.ElideRight
                }

                Button {
                    id: closeButton
                    width: 30
                    height: 30
                    flat: true

                    contentItem: Text {
                        text: "\u2715"
                        font.pixelSize: 20
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        infoPanel.visible = false
                        infoPanel.hidePastTrackRequested()
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 230  // Adjusted for asymmetric padding

            Image {
                anchors {
                    fill: parent
                    leftMargin: 10
                    rightMargin: 10
                    topMargin: 15
                    bottomMargin: 15
                }
                source: "qrc:/shipImg.jpg"
                fillMode: Image.PreserveAspectCrop
            }

            border.width: 1
            border.color: "#cccccc"
        }

        // After the Image component in your RightDock.qml

        Rectangle {
            Layout.fillWidth: true
            height: 60
            color: "#f0f0f0"
            border.color: "#e0e0e0"
            border.width: 1

            Row {
                anchors.fill: parent
                spacing: 1

                Repeater {
                    model: [
                        { label: "MMSI", value: infoPanel.currentShip ? infoPanel.currentShip.mmsi : "N/A" },
                        { label: "Latitude", value: infoPanel.currentShip ? infoPanel.currentShip.latitude.toFixed(4) + "°" : "N/A" },
                        { label: "Longitude", value: infoPanel.currentShip ? infoPanel.currentShip.longitude.toFixed(4) + "°" : "N/A" }
                    ]

                    Rectangle {
                        width: parent.width / 3
                        height: parent.height
                        color: index % 2 === 0 ? "#ffffff" : "#f9f9f9"

                        Column {
                            anchors.centerIn: parent
                            spacing: 5

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: modelData.label
                                font.pixelSize: 12
                                font.bold: true
                                color: "#555555"
                            }

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: modelData.value
                                font.pixelSize: 14
                                color: "#333333"
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 60
            color: "#f0f0f0"
            border.color: "#e0e0e0"
            border.width: 1

            Row {
                anchors.fill: parent
                spacing: 1

                Repeater {
                    model: [
                        {
                            label: "Nav Status",
                            value: infoPanel.currentShip ? shipTableModel.getTrackNavStatus(infoPanel.currentShip.track_nav_status__id) : "N/A"
                        },
                        {
                            label: "Speed / Course",
                            value: infoPanel.currentShip ?
                                (infoPanel.currentShip.speed_over_ground !== undefined ?
                                    infoPanel.currentShip.speed_over_ground.toFixed(1) + " kts" : "N/A") +
                                " / " +
                                (infoPanel.currentShip.course_over_ground !== undefined ?
                                    infoPanel.currentShip.course_over_ground.toFixed(1) + "°" : "N/A")
                                : "N/A"
                        }
                    ]

                    Rectangle {
                        width: parent.width / 2
                        height: parent.height
                        color: index % 2 === 0 ? "#ffffff" : "#f9f9f9"

                        Column {
                            anchors.centerIn: parent
                            spacing: 5

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: modelData.label
                                font.pixelSize: 12
                                font.bold: true
                                color: "#555555"
                            }

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: modelData.value
                                font.pixelSize: 14
                                color: "#333333"
                            }
                        }
                    }
                }
            }
        }


        // Ship details
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            ColumnLayout {
                width: parent.width
                spacing: 10

                Text {
                    Layout.fillWidth: true
                    Layout.margins: 10
                    text: "Total Valid Ships: " + Object.keys(shipData.ships).filter(key => isValidShip(shipData.ships[key])).length
                    font.pixelSize: 14
                }

                Text {
                    id: shipDetailsText
                    Layout.fillWidth: true
                    Layout.margins: 10
                    wrapMode: Text.WordWrap
                    text: {
                        if (!infoPanel.currentShip) return "Click on a ship to see details";
                        let ship = infoPanel.currentShip;
                        return `MMSI: ${ship.mmsi || 'N/A'}
Latitude: ${ship.latitude.toFixed(4)}
Longitude: ${ship.longitude.toFixed(4)}
Speed: ${ship.speed_over_ground !== undefined ? ship.speed_over_ground + ' knots' : 'N/A'}
Course: ${ship.course_over_ground !== undefined ? ship.course_over_ground + '°' : 'N/A'}
Nav Status: ${ship.track_nav_status__id !== undefined ? ship.track_nav_status__id : 'N/A'}
Last Update: ${ship.sensor_timestamp}
                                `;
                        }
                    font.pixelSize: 12
                }

                Item { Layout.fillHeight: true } // Spacer
            }
        }

        // After the MMSI, Latitude, Longitude section


        // Buttons at the bottom
        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 10
            spacing: 10

            Button {
                Layout.fillWidth: true
                text: "Show Trail"
                onClicked: {
                    if (currentShipUuid) {
                        infoPanel.fetchTrackHistoryRequested(currentShipUuid)
                        infoPanel.shipDetailsRequested(currentShip.track_name, currentShip.latitude, currentShip.longitude)
                    } else {
                        console.error("No ship UUID available")
                    }
                }
            }

            Button {
                Layout.fillWidth: true
                text: "Hide Trail"
                onClicked: {
                    infoPanel.hidePastTrackRequested()
                }
            }
        }
    }

    onClosed: {
        if (infoPanel.visible) {
            open()
        }
    }
}
