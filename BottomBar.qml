import QtQuick
import QtQuick.Layouts

Rectangle {
    id: bottomBar
    width: parent.width
    height: 50
    color: "#282c34"
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 20
    border.color: "#555555"
    border.width: 1

    function updateCoordinates(lat, lon) {
        latitudeText.text = formatDMS(lat, true)
        longitudeText.text = formatDMS(lon, false)
    }

    function updateBearing(bearing) {
        bearingText.text = "Bearing: " + bearing + "°"
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 10
        anchors.verticalCenter: parent.verticalCenter

        Text {
            text: "Ship"
            font.pixelSize: 14
            font.family: "Courier New"
            color: "#FFFFFF"
            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: 40
        }

        // Static Latitude and Longitude
        Text {
            text: "33° 54.6567' E"
            font.pixelSize: 14
            font.family: "Courier New"
            color: "#FFFFFF"
            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: 120
        }

        Text {
            text: "151° 17.3950' N"
            font.pixelSize: 14
            font.family: "Courier New"
            color: "#FFFFFF"
            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: 120
        }
        Item { Layout.fillWidth: true }

        Item {
            Layout.preferredWidth: 140
            Layout.preferredHeight: parent.height

            Text {
                id: sogCogText
                text: "SOG---COG---°"
                font.pixelSize: 14
                font.family: "Courier New"
                color: "#FFFFFF"
                anchors.left: parent.left
                anchors.leftMargin: -20  // Adjust this value to move left
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        // Dynamic Latitude and Longitude
        Text {
            id: latitudeText
            text: formatDMS(0, true)  // Initial value, will be updated
            font.pixelSize: 14
            font.family: "Courier New"
            color: "#FFFFFF"
            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: 120
        }

        Text {
            id: longitudeText
            text: formatDMS(0, false)  // Initial value, will be updated
            font.pixelSize: 14
            font.family: "Courier New"
            color: "#FFFFFF"
            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: 120
        }

        Text {
            id: bearingText
            text: "Bearing: ---°"
            font.pixelSize: 14
            font.family: "Courier New"
            color: "#1E90FF"
            Layout.alignment: Qt.AlignRight
            Layout.preferredWidth: 120
        }
    }

    function formatDMS(coordinate, isLatitude) {
        var absolute = Math.abs(coordinate);
        var degrees = Math.floor(absolute);
        var minutesNotTruncated = (absolute - degrees) * 60;
        var minutes = minutesNotTruncated.toFixed(4);
        var direction = isLatitude ? (coordinate >= 0 ? "E" : "W") : (coordinate >= 0 ? "N" : "S");

        return degrees + "° " + minutes + "' " + direction;
    }
}
