// RightDock.qml
import QtQuick
import QtQuick.Controls

Drawer {
    id: infoPanel
    width: 250
    height: parent.height
    edge: Qt.RightEdge
    modal: false
    interactive: true
    opacity: 0.8
    visible: false // Initially hidden

    property var currentShip: null

    Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

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
                return `${vesselName}\n${mmsi}\n${imo}\n${lat}\n${lon}\n${speed}\n${course}`;
            }
            font.pixelSize: 12
        }

    }
}
