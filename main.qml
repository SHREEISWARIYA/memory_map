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

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        Button {
            text: "Fetch Ship Data"
            onClicked: {
                shipDataModel.fetchShipData()
                bottomDrawer.open()
            }
            Layout.alignment: Qt.AlignHCenter
        }

        Text {
            visible: shipDataModel.isLoading
            text: "Loading..."
            Layout.alignment: Qt.AlignHCenter
        }
    }

    Drawer {
        id: bottomDrawer
        width: parent.width
        height: parent.height * 0.4
        edge: Qt.BottomEdge

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            TableView {
                id: tableView
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                model: shipDataModel.tableModel

                columnWidthProvider: function(column) { return 150; }

                // Header row
                Row {
                    id: headerRow
                    y: tableView.contentY
                    z: 2
                    Repeater {
                        model: tableView.columns
                        Rectangle {
                            width: tableView.columnWidthProvider(modelData)
                            height: 35
                            color: "lightgray"
                            border.width: 1
                            border.color: "gray"

                            Text {

                                anchors.centerIn: parent
                                text: shipDataModel.tableModel.headerData(modelData, Qt.Horizontal)
                                font.bold: true
                            }
                        }
                    }
                }

                // Data rows
                delegate: Rectangle {
                    implicitWidth: 150
                    implicitHeight: 35
                    border.width: 1
                    border.color: "lightgray"

                    Text {
                        wrapMode: Text.Wrap
                        anchors.centerIn: parent
                        text: display
                        elide: Text.ElideRight
                    }
                }

                // Adjust the top margin to account for the header row
                topMargin: headerRow.height
            }

            // Pagination controls
            RowLayout {
                Layout.alignment: Qt.AlignHCenter




                // Entries info text
                Text {
                    id: entriesInfoText
                    Layout.alignment: Qt.AlignHCenter

                    text: {
                        if (shipDataModel.totalShips > 0) {
                            var start = (shipDataModel.currentPage - 1) * shipDataModel.itemsPerPage + 1;
                            var end = Math.min(start + shipDataModel.itemsPerPage - 1, shipDataModel.totalShips);
                            return "Showing " + start + " to " + end + " of " + shipDataModel.totalShips + " entries";
                        } else {
                            return "Showing 0 to 0 of 0 entries";
                        }
                    }
                }
                Item {
                        Layout.fillWidth: true // This will take up all available space
                    }
                PaginationControl {
                    currentPage: shipDataModel.currentPage
                    totalPages: Math.max(1, Math.ceil(shipDataModel.totalShips / shipDataModel.itemsPerPage))
                    onPageChanged: shipDataModel.currentPage = page
                }
                Item {
                        Layout.fillWidth: true // This will take up all available space
                    }
                RowLayout{
                    Label { text: "Items per page:" }

                    ComboBox {
                        id: itemsPerPageComboBox
                        model: [5, 10, 15, 20, 25, 30] // Options for items per page
                        currentIndex: model.indexOf(shipDataModel.itemsPerPage)
                        onCurrentIndexChanged: {
                            shipDataModel.itemsPerPage = model[currentIndex];
                        }
                    }
                }


            }


        }
    }

    Component.onCompleted: {
        console.log("QML component completed")
        shipData.fetchShips()
        shipDataModel.fetchShipData()
    }
}

