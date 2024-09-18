import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Layouts
import QtLocation
import QtPositioning

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "Ship Tracking Map"

    // Import the RightDock component
    RightDock {
        id: infoPanel // You can reference it by this ID
    }

    function isValidShip(ship) {
        return ship && ship.vessel_name && ship.latitude !== undefined && ship.longitude !== undefined;
    }

    Map {
        id: mapview
        anchors.fill: parent
        plugin: Plugin {
            name: "osm"
            PluginParameter { name: "osm.mapping.providersrepository.disabled"; value: false }
        }

        activeMapType: supportedMapTypes[supportedMapTypes.length - 1]
        center: QtPositioning.coordinate(0, 0)  // Set an initial center
        minimumZoomLevel: 1
        maximumZoomLevel: 18
        zoomLevel: 3

        // Map Movement
        property geoCoordinate startCentroid
        PinchHandler {
            id: pinch
            target: null
            onActiveChanged: if (active) {
                mapview.startCentroid = mapview.toCoordinate(pinch.centroid.position, false)
            }
            onScaleChanged: (delta) => {
                mapview.zoomLevel += Math.log2(delta)
                mapview.alignCoordinateToPoint(mapview.startCentroid, pinch.centroid.position)
            }
            onRotationChanged: (delta) => {
                mapview.bearing -= delta
                mapview.alignCoordinateToPoint(mapview.startCentroid, pinch.centroid.position)
            }
            grabPermissions: PointerHandler.TakeOverForbidden
        }
        WheelHandler {
            id: wheel
            acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland"
                             ? PointerDevice.Mouse | PointerDevice.TouchPad
                             : PointerDevice.Mouse
            rotationScale: 1/120
            property: "zoomLevel"
        }
        DragHandler {
            id: drag
            target: null
            onTranslationChanged: (delta) => mapview.pan(-delta.x, -delta.y)
        }
        Shortcut {
            enabled: mapview.zoomLevel < mapview.maximumZoomLevel
            sequence: StandardKey.ZoomIn
            onActivated: mapview.zoomLevel = Math.round(mapview.zoomLevel + 1)
        }
        Shortcut {
            enabled: mapview.zoomLevel > mapview.minimumZoomLevel
            sequence: StandardKey.ZoomOut
            onActivated: mapview.zoomLevel = Math.round(mapview.zoomLevel - 1)
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            property var coordinate: mapview.toCoordinate(Qt.point(mouseX, mouseY))

            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: function(mouse) {
                if(typeDraw == 0) {
                    //console.log("nothing")
                } else if(typeDraw == 1) {
                    pointspace.mapPointMark(mouse)
                } else if(typeDraw == 2) {
                    //console.log("line")
                    linespace.mapLineMark(mouse)
                } else if(typeDraw == 3) {
                    //console.log("polyon")
                    polygonspace.polygonMark(mouse)
                } else if(typeDraw == 4) {
                    //console.log("circle")
                    circlespace.circleMark(mouse)
                } else {
                    console.log("invalid type")
                }
            }

            Label {
                id: coordLabel
                x: mapview.width - width
                y: mapview.height - height
                text: "lat: %1; lon: %2".arg(parent.coordinate.latitude).arg(parent.coordinate.longitude)
                color: "black"
                font.pixelSize: 12
                background: Rectangle {
                    color: "white"
                    opacity: 0.5
                }
            }
        }

        MapItemView {
            model: shipData.shipList
            delegate: MapQuickItem {
                id: shipItem
                property var shipDetails: shipData.getShipDetails(modelData.uuid)
                visible: shipData.isValidShip(modelData.uuid)
                coordinate: QtPositioning.coordinate(shipDetails.latitude, shipDetails.longitude)
                anchorPoint.x: image.width/2
                anchorPoint.y: image.height/2

                sourceItem: Image {
                    id: image
                    source: "qrc:/ship.png"
                    width: 18
                    height: 16

                    ToolTip {
                        id: hoverToolTip
                        visible: false // Initially hidden
                        delay: 500 // Half-second delay before showing tooltip
                        timeout: 5000 // Tooltip disappears after 5 seconds if mouse doesn't move

                        contentItem: Column {
                            spacing: 5

                            Text {
                                id: mmsiText
                                text: "MMSI: "
                                font.pixelSize: 12
                                color: "white"
                                wrapMode: Text.WordWrap
                                anchors.topMargin: -5 // Move MMSI slightly up
                            }
                            Text {
                                id: vesselNameText
                                text: "Vessel Name: "
                                font.pixelSize: 12
                                color: "white"
                                wrapMode: Text.WordWrap
                            }
                            Text {
                                id: latitudeText
                                text: "Latitude: "
                                font.pixelSize: 12
                                color: "white"
                                wrapMode: Text.WordWrap
                            }
                            Text {
                                id: longitudeText
                                text: "Longitude: "
                                font.pixelSize: 12
                                color: "white"
                                wrapMode: Text.WordWrap
                            }
                        }

                        background: Rectangle {
                            color: "black"
                            opacity: 0.8
                            radius: 5
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true

                        property string shipUuid: modelData.uuid

                        // Update hover information on mouse enter
                        onEntered: {
                            let shipDetails = shipData.getShipDetails(shipUuid);
                            mmsiText.text = "MMSI: " + shipDetails.mmsi;
                            vesselNameText.text = "Vessel Name: " + shipDetails.vessel_name;
                            latitudeText.text = "Latitude: " + shipDetails.latitude.toFixed(4);
                            longitudeText.text = "Longitude: " + shipDetails.longitude.toFixed(4);
                            hoverToolTip.visible = true; // Show the tooltip
                            hoverToolTip.x = mouse.x; // Position it at the mouse cursor
                            hoverToolTip.y = mouse.y; // Position it at the mouse cursor
                        }

                        onClicked: {
                            let shipDetails = shipData.getShipDetails(shipUuid);
                            console.log("Clicked ship UUID:", shipUuid);
                            console.log("Ship MMSI:", shipDetails.mmsi);
                            shipData.printShipDetails(shipUuid);
                            infoPanel.currentShip = shipDetails;
                            infoPanel.visible = true; // Open the drawer when a ship is clicked
                        }

                        onExited: {
                            hoverToolTip.visible = false; // Hide the tooltip when not hovering
                        }
                    }
                }
            }
        }
    } //map ends



    // Rectangle {
    //     id: mmsiUuidPanel
    //     width: 250
    //     height: parent.height
    //     anchors.left: parent.left
    //     color: "white"
    //     opacity: 0.8

    //     Column {
    //         anchors.fill: parent
    //         anchors.margins: 10
    //         spacing: 10

    //         Text {
    //             anchors.horizontalCenter: parent.horizontalCenter
    //             text: "MMSI-UUID List"
    //             font.pixelSize: 18
    //             font.bold: true
    //         }

    //         ListView {
    //             width: parent.width
    //             height: parent.height - 40  // Subtract some space for the header
    //             model: shipData.mmsiUuidList
    //             clip: true

    //             delegate: ItemDelegate {
    //                 width: parent.width
    //                 height: 60

    //                 Column {
    //                     anchors.fill: parent
    //                     anchors.margins: 5

    //                     Text {
    //                         text: "MMSI: " + modelData.mmsi
    //                         font.bold: true
    //                     }
    //                     Text {
    //                         text: "UUID: " + modelData.uuid
    //                     }
    //                 }
    //             }

    //             ScrollBar.vertical: ScrollBar {}
    //         }
    //     }
    // }


        Component.onCompleted: {
            console.log("QML component completed")
            shipData.fetchShips()
        }



   }
