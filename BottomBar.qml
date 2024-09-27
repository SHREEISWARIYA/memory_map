import QtQuick
import QtQuick.Layouts
import QtQuick.Controls 2.15

Rectangle {
    id: bottomBar
    width: parent.width
    height: 50
    z:10
    color: "#282c34"
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 20
    border.color: "#555555"
    border.width: 1

    // Define the signal here
    signal fetchShipDataRequested()


    function updateCoordinates(lat, lon) {
        latitudeText.text = formatDMS(lat, true)
        longitudeText.text = formatDMS(lon, false)
    }

    function updateBearing(bearing) {
        bearingText.text = "Bearing: " + bearing + "°"
    }

    // Add this property
    property real currentDisplacement: 0

    onCurrentDisplacementChanged: {
            console.log("BottomBar displacement changed:", currentDisplacement)
            displacementText.text = "Displacement: " + currentDisplacement.toFixed(2) + " Nmi"
    }


    RowLayout {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 10
        anchors.verticalCenter: parent.verticalCenter

        Button {
            id: shipButton
            text: "Ship"
            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: 80
            Layout.preferredHeight: 40

            background: Rectangle {
                color: shipButton.pressed ? "#3a3a3a" : "#4a4a4a"
                radius: 20
                border.color: "#555555"
                border.width: 1

                layer.enabled: true
            }

            contentItem: Text {
                text: shipButton.text
                font.pixelSize: 14
                font.family: "Courier New"
                color: "#FFFFFF"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true

                onClicked: {
                    // Add your click handling logic here
                    console.log("Ship button clicked")
                    bottomBar.fetchShipDataRequested()
                }

                onPressed: parent.scale = 0.95
                onReleased: parent.scale = 1.0

                onEntered: parent.opacity = 0.8
                onExited: parent.opacity = 1.0
            }
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

        Text {
                    id: displacementText
                    text: "Displacement: 0 Nmi"
                    font.pixelSize: 14
                    font.family: "Courier New"
                    color: "#FFFFFF"
                    Layout.alignment: Qt.AlignRight
                    Layout.preferredWidth: 150
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
