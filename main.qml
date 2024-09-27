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
    width: 1920 / 2
    height: 1080 / 2
    title: "Ship Tracking Map"

    // Shortcut to toggle full screen
    Shortcut {
        sequences: ["Esc", "F11"]
        onActivated: toggleFullScreen()
    }

    // Function to toggle full screen
    function toggleFullScreen() {
        if (window.visibility === Window.FullScreen)
            window.showNormal()
        else
            window.showFullScreen()
    }

    // Import the RightDock component
    RightDock {
        id: infoPanel // You can reference it by this ID
    }

    // Import the Shipstable component
    Shipstable {
        id: bottomDrawer
    }

    function isValidShip(ship) {
        return ship && ship.track_name && ship.latitude !== undefined
                && ship.longitude !== undefined
    }

    function calculateBearing(lat1, lon1, lat2, lon2) {
        var toRadians = function (degree) {
            return degree * Math.PI / 180
        }
        var toDegrees = function (radian) {
            return radian * 180 / Math.PI
        }

        var dLon = toRadians(lon2 - lon1)
        var lat1Rad = toRadians(lat1)
        var lat2Rad = toRadians(lat2)

        var y = Math.sin(dLon) * Math.cos(lat2Rad)
        var x = Math.cos(lat1Rad) * Math.sin(lat2Rad) - Math.sin(
                    lat1Rad) * Math.cos(lat2Rad) * Math.cos(dLon)

        var brng = toDegrees(Math.atan2(y, x))
        brng = (brng + 180) % 360 // Normalize to 0-360
        return brng.toFixed(1)
    }

    // Add this function to calculate distance in nautical miles
    function calculateDistance(lat1, lon1, lat2, lon2) {
        var R = 6371
        // Radius of the Earth in km
        var dLat = (lat2 - lat1) * Math.PI / 180
        var dLon = (lon2 - lon1) * Math.PI / 180
        var a = Math.sin(dLat / 2) * Math.sin(dLat / 2) + Math.cos(
                    lat1 * Math.PI / 180) * Math.cos(
                    lat2 * Math.PI / 180) * Math.sin(
                    dLon / 2) * Math.sin(dLon / 2)
        var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a))
        var distanceKm = R * c
        return distanceKm * 0.539957 // Convert km to nautical miles
    }

    Map {
        id: mapview
        anchors.fill: parent
        plugin: Plugin {
            name: "osm"

            // PluginParameter {
            //     name: "osm.mapping.custom.host"
            //     value: "http://localhost:2080/tiles/"
            // }
            PluginParameter {
                name: "osm.mapping.providersrepository.disabled"
                value: false
            }
        }

        activeMapType: supportedMapTypes[supportedMapTypes.length - 1]
        center: QtPositioning.coordinate(0, 0) // Set an initial center
        minimumZoomLevel: 1
        maximumZoomLevel: 18
        zoomLevel: 3

        // Map Movement
        property geoCoordinate startCentroid
        PinchHandler {
            id: pinch
            target: null
            onActiveChanged: if (active) {
                                 mapview.startCentroid = mapview.toCoordinate(
                                             pinch.centroid.position, false)
                             }
            onScaleChanged: delta => {
                                mapview.zoomLevel += Math.log2(delta)
                                mapview.alignCoordinateToPoint(
                                    mapview.startCentroid,
                                    pinch.centroid.position)
                            }
            onRotationChanged: delta => {
                                   mapview.bearing -= delta
                                   mapview.alignCoordinateToPoint(
                                       mapview.startCentroid,
                                       pinch.centroid.position)
                               }
            grabPermissions: PointerHandler.TakeOverForbidden
        }
        WheelHandler {
            id: wheel
            acceptedDevices: Qt.platform.pluginName === "cocoa"
                             || Qt.platform.pluginName
                             === "wayland" ? PointerDevice.Mouse
                                             | PointerDevice.TouchPad : PointerDevice.Mouse
            rotationScale: 1 / 120
            property: "zoomLevel"
        }
        DragHandler {
            id: drag
            target: null
            onTranslationChanged: delta => mapview.pan(-delta.x, -delta.y)
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
            property var coordinate: mapview.toCoordinate(Qt.point(mouseX,
                                                                   mouseY))
            property bool shipClicked: false

            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: function (mouse) {
                if (!shipClicked) {
                    shipLabel.visible = false
                    markerItem.visible = false // Hide the marker (ripple effect)
                }
                shipClicked = false // Reset the flag immediately after checking

                if (typeDraw == 0) {

                    //console.log("nothing")
                } else if (typeDraw == 1) {
                    pointspace.mapPointMark(mouse)
                } else if (typeDraw == 2) {
                    linespace.mapLineMark(mouse)
                } else if (typeDraw == 3) {
                    polygonspace.polygonMark(mouse)
                } else if (typeDraw == 4) {
                    circlespace.circleMark(mouse)
                } else {
                    console.log("invalid type")
                }
            }

            onPositionChanged: {
                bottomBar.updateCoordinates(coordinate.latitude,
                                            coordinate.longitude)
                updatePositionAndBearing(coordinate.latitude,
                                         coordinate.longitude)

                // Calculate and update displacement
                var displacement = calculateDistance(
                            coordinate.latitude, coordinate.longitude,
                            fixedMarker.coordinate.latitude,
                            fixedMarker.coordinate.longitude)
                bottomBar.updateDisplacement(displacement.toFixed(2))
            }

            Label {
                id: coordLabel
                x: mapview.width - width
                y: mapview.height - height
                text: "lat: %1; lon: %2".arg(parent.coordinate.latitude).arg(
                          parent.coordinate.longitude)
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
                property var shipDetails: shipData.getShipDetails(
                                              modelData.uuid)
                visible: shipData.isValidShip(modelData.uuid)
                coordinate: QtPositioning.coordinate(shipDetails.latitude,
                                                     shipDetails.longitude)
                anchorPoint.x: image.width / 2
                anchorPoint.y: image.height / 2

                sourceItem: Image {
                    id: image
                    source: "qrc:/ship.png"
                    width: 18
                    height: 16

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            mapview.children[0].shipClicked = true // Set the flag
                            console.log("Clicked MMSI:", shipDetails.mmsi)
                            console.log("Latitude:", shipDetails.latitude)
                            console.log("Longitude:", shipDetails.longitude)

                            if (shipDetails.latitude && shipDetails.longitude) {
                                handleShipClick(shipDetails.latitude,
                                                shipDetails.longitude,
                                                shipDetails.track_name,
                                                shipDetails.mmsi)
                            } else {
                                console.error("Invalid latitude or longitude")
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
                            let shipDetails = shipData.getShipDetails(shipUuid)
                            mmsiText.text = "MMSI: " + shipDetails.mmsi
                            vesselNameText.text = "Vessel Name: " + shipDetails.track_name
                            latitudeText.text = "Latitude: " + shipDetails.latitude.toFixed(
                                        4)
                            longitudeText.text = "Longitude: " + shipDetails.longitude.toFixed(
                                        4)
                            hoverToolTip.visible = true // Show the tooltip
                            hoverToolTip.x = mouse.x // Position it at the mouse cursor
                            hoverToolTip.y = mouse.y // Position it at the mouse cursor
                        }

                        onClicked: {
                            let shipDetails = shipData.getShipDetails(shipUuid)
                            console.log("Clicked ship UUID:", shipUuid)
                            console.log("Ship MMSI:", shipDetails.mmsi)
                            shipData.printShipDetails(shipUuid)
                            infoPanel.currentShip = shipDetails
                            infoPanel.visible = true // Open the drawer when a ship is clicked

                            // Fetch track history
                            pastTrack.fetchTrackHistory(shipUuid, 900)
                        }

                        onExited: {
                            hoverToolTip.visible = false // Hide the tooltip when not hovering
                        }
                    }
                }
            }
        }

        Component {
            id: mapCircleComponent
            MapCircle {
                radius: 50
                color: 'red'
                border.color: 'black'
                border.width: 1
            }
        }

        // Add this MapQuickItem for the marker
        MapQuickItem {
            id: markerItem
            anchorPoint.x: marker.width / 2
            anchorPoint.y: marker.height / 2
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
                    width: marker.width * 0.6 // Reduced to 80% of the marker size
                    height: marker.height * 0.6 // Reduced to 80% of the marker size
                    color: 'transparent'
                    border.color: 'red'
                    border.width: 2
                    radius: width / 2

                    // Ripple animation
                    SequentialAnimation on scale {
                        loops: Animation.Infinite
                        PropertyAnimation {
                            to: 1.5
                            duration: 1000
                        }
                        PropertyAnimation {
                            to: 1.0
                            duration: 0
                        }
                    }
                    SequentialAnimation on opacity {
                        loops: Animation.Infinite
                        PropertyAnimation {
                            to: 0
                            duration: 1000
                        }
                        PropertyAnimation {
                            to: 0.5
                            duration: 0
                        }
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
            coordinate: QtPositioning.coordinate(
                            -23.289792,
                            40.816407) // 23° 17.3875' S, 40° 48.9844' E
            anchorPoint.x: image.width / 2
            anchorPoint.y: image.height
            sourceItem: Image {
                id: image2
                source: "qrc:/marker.png" // Make sure to add a marker image to your resources
                width: 32
                height: 32
            }
        }
    } //map ends

    //SIDEBAR =============================================================================================================================================================================
    Column {
        spacing: 10
        padding: 6
        anchors.top: parent.top
        anchors.topMargin: 40

        // Button {
        //     text: "Fetch Ship Data"
        //     onClicked: {
        //         shipDataModel.fetchShipData()
        //         bottomDrawer.show()
        //     }
        //     anchors.top: parent.top
        //     anchors.left: parent.left
        //     anchors.margins: 10
        // }
        Button {
            width: 33
            height: 33
            background: Rectangle {
                color: "#555" // Background color
                radius: 8 // Rounded corners
                border.color: "#666" // Subtle border
                border.width: 1
            }
            contentItem: Image {
                source: "qrc:/ship-3.png"
                height: 20
                width: 20
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
            }
            onClicked: {
                // Change theme logic
                mapview.center = fixedMarker.coordinate
            }
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true

                onClicked: {
                    // Center the map on the fixedMarker's coordinate
                    mapview.center = fixedMarker.coordinate
                    mapview.zoomLevel = 15 // Optional: Adjust the zoom level as needed
                }
            }
        }

        Text {
            visible: shipDataModel.isLoading
            text: "Loading..."
            Layout.alignment: Qt.AlignHCenter
        }

        Button {
            width: 33
            height: 33
            background: Rectangle {
                color: "#555" // Background color
                radius: 8 // Rounded corners
                border.color: "#666" // Subtle border
                border.width: 1
            }
            contentItem: Image {
                source: "qrc:/pencil.png"
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
            }
            onClicked: {

                // Marker logic
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {

                    // Marker logic
                }
                onPressedChanged: {
                    if (pressed) {
                        parent.background.color = "#444" // Darker on press
                    } else {
                        parent.background.color = "#555" // Original color
                    }
                }
            }
        }

        Button {
            width: 33
            height: 33
            background: Rectangle {
                color: "#555" // Background color
                radius: 8 // Rounded corners
                border.color: "#666" // Subtle border
                border.width: 1
            }
            contentItem: Image {
                source: "qrc:/route.png"
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
            }
            onClicked: {

                // Marker logic
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {

                    // Marker logic
                }
                onPressedChanged: {
                    if (pressed) {
                        parent.background.color = "#444" // Darker on press
                    } else {
                        parent.background.color = "#555" // Original color
                    }
                }
            }
        }

        Button {
            width: 33
            height: 33
            background: Rectangle {
                color: "#555" // Background color
                radius: 8 // Rounded corners
                border.color: "#666" // Subtle border
                border.width: 1
            }
            contentItem: Image {
                source: "qrc:/map.png"
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
            }
            onClicked: {
                mapDialog.open()
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    mapDialog.open()
                }
                onPressedChanged: {
                    if (pressed) {
                        parent.background.color = "#444" // Darker on press
                    } else {
                        parent.background.color = "#555" // Original color
                    }
                }
            }
        }
        MapDialog {
            id: mapDialog
            x: window.width - width - 10
            y: 0
            width: 500 // Adjust the width as needed
            height: parent.height
        }

        Button {
            width: 33
            height: 33
            background: Rectangle {
                color: "#555" // Background color
                radius: 8 // Rounded corners
                border.color: "#666" // Subtle border
                border.width: 1
            }
            contentItem: Image {
                source: "qrc:/changetheme.png"
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
            }
            onClicked: {

                // Marker logic
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {

                    // Marker logic
                }
                onPressedChanged: {
                    if (pressed) {
                        parent.background.color = "#444" // Darker on press
                    } else {
                        parent.background.color = "#555" // Original color
                    }
                }
            }
        }

        Button {
            width: 33
            height: 33
            background: Rectangle {
                color: "#555" // Background color
                radius: 8 // Rounded corners
                border.color: "#666" // Subtle border
                border.width: 1
            }
            contentItem: Image {
                source: "qrc:/settings.png"
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
            }
            onClicked: {

                // Marker logic
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {

                    // Marker logic
                }
                onPressedChanged: {
                    if (pressed) {
                        parent.background.color = "#444" // Darker on press
                    } else {
                        parent.background.color = "#555" // Original color
                    }
                }
            }
        }

        Button {
            width: 33
            height: 33
            background: Rectangle {
                color: "#555" // Background color
                radius: 8 // Rounded corners
                border.color: "#666" // Subtle border
                border.width: 1
            }
            contentItem: Image {
                source: "qrc:/help.png"
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
            }
            onClicked: {

                // Marker logic
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {

                    // Marker logic
                }
                onPressedChanged: {
                    if (pressed) {
                        parent.background.color = "#444" // Darker on press
                    } else {
                        parent.background.color = "#555" // Original color
                    }
                }
            }
        }

        Button {
            width: 33
            height: 33
            background: Rectangle {
                color: "#555" // Background color
                radius: 8 // Rounded corners
                border.color: "#666" // Subtle border
                border.width: 1
            }
            contentItem: Image {
                source: "qrc:/trackList.png"
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
            }
            onClicked: {

                // Marker logic
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {

                    // Marker logic
                }
                onPressedChanged: {
                    if (pressed) {
                        parent.background.color = "#444" // Darker on press
                    } else {
                        parent.background.color = "#555" // Original color
                    }
                }
            }
        }
    }

    // ===================================================================================================================================================================

    // function handleTrackHistoryFetched(trackHistory) {
    //         console.log(" ******************************************** Track history fetched: ***", trackHistory);

    //         // Remove previous circles
    //         for (var i = mapview.children.length - 1; i >= 0; i--) {
    //             if (mapview.children[i] instanceof MapCircle) {
    //                 mapview.children[i].destroy();
    //             }
    //         }

    //         // Add new circles
    //         var trackList = trackHistory.trackHistory;
    //         for (i = 0; i < trackList.length; i++) {
    //             var point = trackList[i];
    //             var coordinate = QtPositioning.coordinate(point.latitude, point.longitude);
    //             console.log("Creating MapCircle at:", coordinate);

    //             // Add a circle for each point
    //             var mapCircle = mapCircleComponent.createObject(mapview, {
    //                 center: coordinate
    //             });
    //             if (mapCircle === null) {
    //                 console.error("Failed to create MapCircle");
    //             } else {
    //                 console.log("MapCircle created at:", coordinate);
    //             }
    //         }

    //         // Center the map on the first point
    //         if (trackList.length > 0) {
    //             mapview.center = QtPositioning.coordinate(trackList[0].latitude, trackList[0].longitude);
    //         }
    //     }
    function handleShipClick(latitude, longitude, shipName, mmsi) {
        console.log("Received latitude:", latitude, "longitude:", longitude)

        if (latitude && longitude) {
            var coordinate = QtPositioning.coordinate(latitude, longitude)
            console.log("Created coordinate:", coordinate)

            if (mapview) {
                mapview.center = coordinate
                mapview.zoomLevel = 10
                console.log("Map centered at:", mapview.center)
            } else {
                console.error("mapview is not defined")
            }

            // Show and position the ship label
            shipLabel.coordinate = coordinate
            shipLabel.sourceItem.children[0].children[0].text = "Name: " + shipName
            shipLabel.sourceItem.children[0].children[1].text = "MMSI: " + mmsi
            shipLabel.sourceItem.children[0].children[2].text = "Lat: " + latitude.toFixed(
                        6)
            shipLabel.sourceItem.children[0].children[3].text = "Lon: " + longitude.toFixed(
                        6)
            shipLabel.visible = true

            // Adjust these values to fine-tune the label position
            var xOffset = 30
            // Pixels to the right
            var yOffset = -50

            // Pixels up
            shipLabel.anchorPoint.x = -xOffset
            shipLabel.anchorPoint.y = shipLabel.sourceItem.height + yOffset

            // Show the marker (ripple effect)
            markerItem.coordinate = coordinate
            markerItem.visible = true
        } else {
            console.error("Invalid latitude or longitude")
        }
    }

    Component.onCompleted: {
        console.log("QML component completed")
        shipData.fetchShips()
        shipDataModel.fetchShipData()
        bottomBar.updateCoordinates(mapview.center.latitude,
                                    mapview.center.longitude)
    }

    function formatDMS(coordinate, isLatitude) {
        var absolute = Math.abs(coordinate)
        var degrees = Math.floor(absolute)
        var minutesNotTruncated = (absolute - degrees) * 60
        var minutes = minutesNotTruncated.toFixed(4)
        var direction = isLatitude ? (coordinate >= 0 ? "E" : "W") : (coordinate >= 0 ? "N" : "S")

        return degrees + "° " + minutes + "' " + direction
    }

    function updatePositionAndBearing(lat, lon) {
        var markerLat = fixedMarker.coordinate.latitude
        var markerLon = fixedMarker.coordinate.longitude

        bottomBar.updateCoordinates(lat, lon)
        var bearing = calculateBearing(lat, lon, markerLat, markerLon)
        bottomBar.updateBearing(bearing)

        // Calculate and update displacement
        var displacement = calculateDistance(lat, lon, markerLat, markerLon)
        //console.log("Calculated displacement in updatePositionAndBearing:", displacement.toFixed(2))
        bottomBar.currentDisplacement = displacement // Update this line
    }

    // ... existing code ...

    // ... existing code ...
    BottomBar {
        id: bottomBar

        // Add this property
        property real currentDisplacement: 0

        // Modify the updateDisplacement function
        function updateDisplacement(displacement) {
            //console.log("BottomBar updateDisplacement called in main.qml:", displacement)
            currentDisplacement = displacement
        }

        onFetchShipDataRequested: {
            shipDataModel.fetchShipData()
            bottomDrawer.open()
        }
    }

    Connections {
        target: backgroundProcessor
        function onProcessingComplete() {
            console.log("Background processing complete")
            // Update UI or perform any other actions
        }
    }
}
