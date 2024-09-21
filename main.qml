import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Layouts
import QtLocation
import QtPositioning
import Qt.labs.platform as Platform

ApplicationWindow {
    id: window
    visible: true
    visibility: Window.FullScreen
    width: 1920/2
    height: 1080/2
    title: "Ship Tracking Map"

    // Shortcut to toggle full screen
    Shortcut {
        sequences: ["Esc", "F11"]
        onActivated: toggleFullScreen()
    }

    // Function to toggle full screen
    function toggleFullScreen() {
        if (window.visibility === Window.FullScreen)
            window.showNormal();
        else
            window.showFullScreen();
    }

    // Import the RightDock component
    RightDock {
        id: infoPanel // You can reference it by this ID
    }

    function isValidShip(ship) {
        return ship && ship.vessel_name && ship.latitude !== undefined && ship.longitude !== undefined;
    }

        function calculateBearing(lat1, lon1, lat2, lon2) {
            var toRadians = function(degree) {
                return degree * Math.PI / 180;
            }
            var toDegrees = function(radian) {
                return radian * 180 / Math.PI;
            }

            var dLon = toRadians(lon2 - lon1);
            var lat1Rad = toRadians(lat1);
            var lat2Rad = toRadians(lat2);

            var y = Math.sin(dLon) * Math.cos(lat2Rad);
            var x = Math.cos(lat1Rad) * Math.sin(lat2Rad) -
                    Math.sin(lat1Rad) * Math.cos(lat2Rad) * Math.cos(dLon);

            var brng = toDegrees(Math.atan2(y, x));
            brng = (brng + 180) % 360; // Normalize to 0-360
            return brng.toFixed(1);
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
            property bool shipClicked: false

            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: function(mouse) {
                if (!shipClicked) {
                    shipLabel.visible = false;
                    markerItem.visible = false;  // Hide the marker (ripple effect)
                }
                shipClicked = false;  // Reset the flag immediately after checking

                if(typeDraw == 0) {
                    //console.log("nothing")
                } else if(typeDraw == 1) {
                    pointspace.mapPointMark(mouse)
                } else if(typeDraw == 2) {
                    linespace.mapLineMark(mouse)
                } else if(typeDraw == 3) {
                    polygonspace.polygonMark(mouse)
                } else if(typeDraw == 4) {
                    circlespace.circleMark(mouse)
                } else {
                    console.log("invalid type")
                }
            }

            onPositionChanged: {
                bottomBar.updateCoordinates(coordinate.latitude, coordinate.longitude)
                updatePositionAndBearing(coordinate.latitude, coordinate.longitude)
            }

            Label {
                            id: coordLabel
                            x: mapview.width - width
                            y: mapview.height - height
                            text: "lat: %1; lon: %2".arg(parent.coordinate.latitude).arg(parent.coordinate.longitude)
                            color: "black"
                            font {
                                pixelSize: 12
                            }
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

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            mapview.children[0].shipClicked = true;  // Set the flag
                            console.log("Clicked MMSI:", shipDetails.mmsi);
                            console.log("Latitude:", shipDetails.latitude);
                            console.log("Longitude:", shipDetails.longitude);

                            if (shipDetails.latitude && shipDetails.longitude) {
                                handleShipClick(shipDetails.latitude, shipDetails.longitude, shipDetails.vessel_name, shipDetails.mmsi);
                            } else {
                                console.error("Invalid latitude or longitude");
                            }
                        }
                    }

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

        // Add this MapQuickItem for the marker
        MapQuickItem {
            id: markerItem
            anchorPoint.x: marker.width/2
            anchorPoint.y: marker.height/2
            coordinate: mapview.center
            visible: false

            sourceItem: Image {
                id: marker
                source: "qrc:/marker2.png" // Make sure you have this image in your resources
                width: 40
                height: 40

                // Ripple effect
                Rectangle {
                    id: ripple
                    anchors.centerIn: parent
                    width: marker.width * 0.6  // Reduced to 80% of the marker size
                    height: marker.height * 0.6  // Reduced to 80% of the marker size
                    color: 'transparent'
                    border.color: 'red'
                    border.width: 2
                    radius: width / 2

                    // Ripple animation
                    SequentialAnimation on scale {
                        loops: Animation.Infinite
                        PropertyAnimation { to: 1.5; duration: 1000 }
                        PropertyAnimation { to: 1.0; duration: 0 }
                    }
                    SequentialAnimation on opacity {
                        loops: Animation.Infinite
                        PropertyAnimation { to: 0; duration: 1000 }
                        PropertyAnimation { to: 0.5; duration: 0 }
                    }
                }
            }
        }

        MapQuickItem {
            id: shipLabel
            visible: false
            z: 1 // Ensure it's above other map items

            sourceItem: Rectangle {
                width: shipInfo.width + 20
                height: shipInfo.height + 20
                color: "black"
                opacity: 0.7
                radius: 5

                Column {
                    id: shipInfo
                    anchors.centerIn: parent
                    spacing: 2

                    Text {
                        id: shipNameText
                        color: "white"
                        font.pixelSize: 14
                        font.bold: true
                    }

                    Text {
                        id: shipMmsiText
                        color: "white"
                        font.pixelSize: 12
                    }

                    Text {
                        id: shipLatText
                        color: "white"
                        font.pixelSize: 12
                    }

                    Text {
                        id: shipLonText
                        color: "white"
                        font.pixelSize: 12
                    }
                }
            }
        }

        MapQuickItem {
            id: fixedMarker
            coordinate: QtPositioning.coordinate(-23.289792, 40.816407) // 23° 17.3875' S, 40° 48.9844' E
            anchorPoint.x: image.width/2
            anchorPoint.y: image.height
            sourceItem: Image {
                id: image2
                source: "qrc:/marker.png" // Make sure to add a marker image to your resources
                width: 32
                height: 32
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
                bottomDrawer.show()
            }
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 10
        }

        Text {
            visible: shipDataModel.isLoading
            text: "Loading..."
            Layout.alignment: Qt.AlignHCenter
        }
    }

    Window {
        id: bottomDrawer
        //visibility: Window.FullScreen
        visible: false
        width: 960
        height: 450
        // width:parent.width/2
        // height:parent.heigth/2
        //edge: Qt.BottomEdge
        Shortcut {
            sequences: ["Esc","F11"]
            onActivated: toggleFullScreen()
        }

        // Function to toggle full screen
        function toggleFullScreen() {
            if (bottomDrawer.visibility === Window.FullScreen)
                bottomDrawer.showNormal();
            else
                bottomDrawer.showFullScreen();
        }
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
                                text:  shipDataModel.tableModel.headerData(modelData, Qt.Horizontal)
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
                        anchors.fill: parent
                        anchors.margins: 5
                        text: (display!="Action") ? display : ""
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        color: column === 0 ? "blue" : "black" // Make MMSI blue to look like a hyperlink
                        font.underline: column === 0 // Underline the MMSI
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: column === 0 ? Qt.PointingHandCursor : Qt.ArrowCursor
                        onClicked: {
                            if (column === 0) { // Check if it's the MMSI column
                                var shipData = shipDataModel.tableModel.getShipDataForRow(row);
                                console.log("Clicked MMSI:", shipData.mmsi);
                                bottomDrawer.close()

                                console.log("Latitude:", shipData.latitude, "Longitude:", shipData.longitude);
                                handleShipClick(shipData.latitude, shipData.longitude, shipData.vessel_name, shipData.mmsi);
                            } else {
                                // Hide the ship name label when clicking elsewhere
                                shipLabel.visible = false;
                            }
                        }
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

    function handleShipClick(latitude, longitude, shipName, mmsi) {
        console.log("Received latitude:", latitude, "longitude:", longitude);

        if (latitude && longitude) {
            var coordinate = QtPositioning.coordinate(latitude, longitude);
            console.log("Created coordinate:", coordinate);

            if (mapview) {
                mapview.center = coordinate;
                mapview.zoomLevel = 15;
                console.log("Map centered at:", mapview.center);
            } else {
                console.error("mapview is not defined");
            }

            // Show and position the ship label
            shipLabel.coordinate = coordinate;
            shipLabel.sourceItem.children[0].children[0].text = "Name: " + shipName;
            shipLabel.sourceItem.children[0].children[1].text = "MMSI: " + mmsi;
            shipLabel.sourceItem.children[0].children[2].text = "Lat: " + latitude.toFixed(6);
            shipLabel.sourceItem.children[0].children[3].text = "Lon: " + longitude.toFixed(6);
            shipLabel.visible = true;

            // Adjust these values to fine-tune the label position
            var xOffset = 30;  // Pixels to the right
            var yOffset = -50; // Pixels up

            shipLabel.anchorPoint.x = -xOffset;
            shipLabel.anchorPoint.y = shipLabel.sourceItem.height + yOffset;

            // Show the marker (ripple effect)
            markerItem.coordinate = coordinate;
            markerItem.visible = true;
        } else {
            console.error("Invalid latitude or longitude");
        }
    }

    Component.onCompleted: {
        console.log("QML component completed")
        shipData.fetchShips()
        shipDataModel.fetchShipData()
        bottomBar.updateCoordinates(mapview.center.latitude, mapview.center.longitude)
    }

    function formatDMS(coordinate, isLatitude) {
        var absolute = Math.abs(coordinate);
        var degrees = Math.floor(absolute);
        var minutesNotTruncated = (absolute - degrees) * 60;
        var minutes = minutesNotTruncated.toFixed(4);
        var direction = isLatitude ? (coordinate >= 0 ? "E" : "W") : (coordinate >= 0 ? "N" : "S");

        return degrees + "° " + minutes + "' " + direction;
    }

    function updatePositionAndBearing(lat, lon) {
        var markerLat = fixedMarker.coordinate.latitude
        var markerLon = fixedMarker.coordinate.longitude

        bottomBar.updateCoordinates(lat, lon)
        var bearing = calculateBearing(lat, lon, markerLat, markerLon)
        bottomBar.updateBearing(bearing)
    }

    BottomBar {
        id: bottomBar
    }

}
