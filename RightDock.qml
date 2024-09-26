// // RightDock.qml
// import QtQuick
// import QtQuick.Controls

// Drawer {
//     id: infoPanel
//     width: 250
//     height: parent.height
//     edge: Qt.RightEdge
//     modal: false
//     interactive: true
//     opacity: 0.8
//     visible: false // Initially hidden

//     property var currentShip: null
//     property var mapView: null
//     property var pastTrail: null
//     property string currentShipUuid: ""



//     // Explicitly declare the signal
//     signal fetchTrackHistoryRequested(string uuid)
//     signal shipDetailsRequested(string name, real latitude, real longitude)

//     Column {
//         anchors.fill: parent
//         anchors.margins: 10
//         spacing: 10

//         Text {
//             anchors.horizontalCenter: parent.horizontalCenter
//             text: "Ship Information"
//             font.pixelSize: 18
//             font.bold: true
//         }

//         Text {
//             anchors.horizontalCenter: parent.horizontalCenter
//             text: "Total Valid Ships: " + Object.keys(shipData.ships).filter(key => isValidShip(shipData.ships[key])).length
//             font.pixelSize: 16
//         }

//         Text {
//             width: parent.width
//             wrapMode: Text.WordWrap
//             text: {
//                 if (!infoPanel.currentShip) return "Click on a ship to see details";
//                 let ship = infoPanel.currentShip;
//                 let vesselName = ship.track_name.trim();
//                 let mmsi = ship.mmsi !== undefined ? `MMSI: ${ship.mmsi}` : "MMSI: N/A";
//                 let imo = ship.imo !== undefined ? (ship.imo !== null ? `IMO: ${ship.imo}` : "IMO: Not Assigned") : "IMO: N/A";
//                 let lat = `Lat: ${ship.latitude.toFixed(4)}`;
//                 let lon = `Lon: ${ship.longitude.toFixed(4)}`;
//                 let speed = ship.speed_over_ground !== undefined ? `Speed: ${ship.speed_over_ground} knots` : "Speed: N/A";
//                 let course = ship.course_over_ground !== undefined ? `Course: ${ship.course_over_ground}°` : "Course: N/A";
//                 let uuid = ship.uuid
//                 return `${vesselName}\n${mmsi}\n${imo}\n${lat}\n${lon}\n${speed}\n${course}\n${uuid}`;
//             }
//             font.pixelSize: 12
//         }

//         Button {
//                     id: pastTrackButton
//                     text: "Show Past Track"
//                     onClicked: {
//                         console.log("Requesting track history for UUID:", currentShipUuid)
//                         if (currentShipUuid) {
//                             // Emit the signal with the UUID
//                             infoPanel.fetchTrackHistoryRequested(currentShipUuid)

//                             console.log(" ^^Name:", currentShip.track_name)
//                             console.log(" ^^Latitude:", currentShip.latitude)
//                             console.log(" ^^Longitude:", currentShip.longitude)

//                             // Emit the new signal with ship details
//                             infoPanel.shipDetailsRequested(currentShip.track_name, currentShip.latitude, currentShip.longitude)

//                         } else {
//                             console.error("No ship UUID available")
//                         }
//                     }
//         }


//     }


// }

// RightDock.qml
import QtQuick
import QtQuick.Controls

Drawer {
    id: infoPanel
    width: 250
    height: parent.height
    edge: Qt.RightEdge
    modal: false
    interactive: false  // Set to false to prevent dragging
    opacity: 0.8
    visible: false // Initially hidden

    property var currentShip: null
    property var mapView: null
    property var pastTrail: null
    property string currentShipUuid: ""

    // Explicitly declare the signal
    signal fetchTrackHistoryRequested(string uuid)
    signal shipDetailsRequested(string name, real latitude, real longitude)
    signal hidePastTrackRequested()

    Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Button {
            id: closeButton
            anchors.right: parent.right
            background: Rectangle{
                color: "transparent"
            }
            width:name.width
            height:name.height
            Image {
                id: name
                anchors.horizontalCenter: Qt.AlignHCenter
                anchors.verticalCenter: Qt.AlignVCenter
                source: "qrc:/close.png"
            }

            onClicked: {
                infoPanel.visible = false
                infoPanel.hidePastTrackRequested()
            }
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Ship Information"
            font.pixelSize: 18
            font.bold: true
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Total Valid Ships: " + Object.keys(shipData.ships).filter(key => isValidShip(shipData.ships[key])).length
            font.pixelSize: 16
        }

        Text {
            width: parent.width
            wrapMode: Text.WordWrap
            text: {
                if (!infoPanel.currentShip) return "Click on a ship to see details";
                let ship = infoPanel.currentShip;
                let vesselName = ship.track_name.trim();
                let mmsi = ship.mmsi !== undefined ? `MMSI: ${ship.mmsi}` : "MMSI: N/A";
                let imo = ship.imo !== undefined ? (ship.imo !== null ? `IMO: ${ship.imo}` : "IMO: Not Assigned") : "IMO: N/A";
                let lat = `Lat: ${ship.latitude.toFixed(4)}`;
                let lon = `Lon: ${ship.longitude.toFixed(4)}`;
                let speed = ship.speed_over_ground !== undefined ? `Speed: ${ship.speed_over_ground} knots` : "Speed: N/A";
                let course = ship.course_over_ground !== undefined ? `Course: ${ship.course_over_ground}°` : "Course: N/A";
                let uuid = ship.uuid
                return `${vesselName}\n${mmsi}\n${imo}\n${lat}\n${lon}\n${speed}\n${course}\n${uuid}`;
            }
            font.pixelSize: 12
        }

        Button {
            id: pastTrackButton
            text: "Show Past Track"
            onClicked: {
                console.log("Requesting track history for UUID:", currentShipUuid)
                if (currentShipUuid) {
                    // Emit the signal with the UUID
                    infoPanel.fetchTrackHistoryRequested(currentShipUuid)

                    console.log(" ^^Name:", currentShip.track_name)
                    console.log(" ^^Latitude:", currentShip.latitude)
                    console.log(" ^^Longitude:", currentShip.longitude)

                    // Emit the new signal with ship details
                    infoPanel.shipDetailsRequested(currentShip.track_name, currentShip.latitude, currentShip.longitude)
                } else {
                    console.error("No ship UUID available")
                }
            }
        }

        Button {
                id: hidePastTrackButton
                text: "Hide Past Track"
                onClicked: {
                    infoPanel.hidePastTrackRequested()
                }
        }
    }

    // Override the default close behavior
    onClosed: {
        // Prevent the drawer from closing when clicked outside
        if (infoPanel.visible) {
            open()
        }
    }
}
