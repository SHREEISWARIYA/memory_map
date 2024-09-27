import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Layouts
import QtLocation
import QtPositioning
import Qt.labs.platform as Platform
import QtQuick.Shapes 1.15
import Qt5Compat.GraphicalEffects // Make sure to import this at the top of your QML file

//import com.example 1.0


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


    // // Import the RightDock component
    // RightDock {
    //     id: infoPanel // You can reference it by this ID
    //     mapView: mapview // Make sure this matches your Map id
    //     pastTrail: pastTrail
    // }

    // main.qml
    RightDock {
        id: infoPanel
        onHidePastTrackRequested: {
                hidePastTrack()
            }
    }

    Connections {
            target: infoPanel
            function onFetchTrackHistoryRequested(uuid) {
                console.log("Main.qml: Fetching track history for UUID:", uuid)
                pastTrail.fetchTrackHistory(uuid, 48)
            }
            function onShipDetailsRequested(name, latitude, longitude) {
                    console.log(" ****Ship details requested: ***")
                    console.log(" $$ Name:", name)
                    console.log(" $$ Latitude:", latitude)
                    console.log(" $$ Longitude:", longitude)
                    // You can add any additional processing here, such as updating the map or other UI elements
                    markShipOnMap(name, latitude, longitude)
            }
        }

    // Import the Shipstable component
    Shipstable {
        id: bottomDrawer
    }

    function isValidShip(ship) {
        return ship && ship.track_name && ship.latitude !== undefined && ship.longitude !== undefined;
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

        // Add this function to calculate distance in nautical miles
            function calculateDistance(lat1, lon1, lat2, lon2) {
                var R = 6371; // Radius of the Earth in km
                var dLat = (lat2 - lat1) * Math.PI / 180;
                var dLon = (lon2 - lon1) * Math.PI / 180;
                var a = Math.sin(dLat/2) * Math.sin(dLat/2) +
                        Math.cos(lat1 * Math.PI / 180) * Math.cos(lat2 * Math.PI / 180) *
                        Math.sin(dLon/2) * Math.sin(dLon/2);
                var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));
                var distanceKm = R * c;
                return distanceKm * 0.539957; // Convert km to nautical miles
            }

            Connections {
                    target: pastTrail
                    onTrackHistoryFetched: handleTrackHistoryFetched(uuid, trackHistory)
            }

            function handleTrackHistoryFetched(uuid, trackHistory) {
                console.log("**********************Track history fetched for UUID:", uuid);
                console.log("Track history length:", trackHistory.length);

                // Remove previous track history lines
                var removedCount = 0;
                for (var i = mapview.mapItems.length - 1; i >= 0; i--) {
                    if (mapview.mapItems[i].objectName === "trackHistoryLine") {
                        mapview.removeMapItem(mapview.mapItems[i]);
                        removedCount++;
                    }
                }
                console.log("Removed", removedCount, "previous track history lines");

                // Create the path for the line
                var path = [];

                // Add coordinates to the path
                for (i = 0; i < trackHistory.length; i++) {
                    var point = trackHistory[i];
                    console.log("Point", i + 1, "- Latitude:", point.latitude, "Longitude:", point.longitude);

                    var coordinate = QtPositioning.coordinate(point.latitude, point.longitude);
                    path.push(coordinate);
                }

                // Create and add the line
                if (path.length > 1) {
                    var line = dottedLineComponent.createObject(mapview, {
                        objectName: "trackHistoryLine",
                        path: path
                    });
                    mapview.addMapItem(line);
                    console.log("Added track history line with", path.length, "points");
                } else {
                    console.log("Not enough points to create a line");
                }

                // Center the map on the first point of the track history
                if (trackHistory.length > 0) {
                    mapview.center = QtPositioning.coordinate(trackHistory[0].latitude, trackHistory[0].longitude);
                    mapview.zoomLevel = 12;
                    console.log("Centered map on", mapview.center, "with zoom level", mapview.zoomLevel);
                } else {
                    console.log("No points to center the map on");
                }
            }

            Component {
                id: dottedLineComponent
                MapPolyline {
                    line.width: 3
                    line.color: "red"
                }
            }

            Component {
                id: highlightCircleComponent
                MapQuickItem {
                    anchorPoint.x: circle.width/2
                    anchorPoint.y: circle.height/2
                    sourceItem: Rectangle {
                        id: circle
                        width: 40
                        height: 40
                        radius: width  // This makes the rectangle circular
                        color: "transparent"
                        border.color: "red"
                        border.width: 3
                        opacity: 0.7

                    }
                    zoomLevel: 0
                }
            }

            Component {
                id: shipLabelComponent
                MapQuickItem {
                    anchorPoint.x: text.width/2
                    anchorPoint.y: text.height + 25  // Adjusted to be closer to the circle
                    sourceItem: Text {
                        id: text
                        font.pixelSize: 14
                        font.bold: true
                        color: "white"
                        style: Text.Outline
                        styleColor: "black"
                    }
                    zoomLevel: 0
                }
            }

            function hidePastTrack() {
                // Remove all track history lines and ship markers
                for (var i = mapview.mapItems.length - 1; i >= 0; i--) {
                    var item = mapview.mapItems[i];
                    if (item.objectName === "trackHistoryLine" ||
                        (item.objectName && item.objectName.startsWith("shipMarker_") || item.objectName.startsWith("shipLabel_"))) {
                        mapview.removeMapItem(item);
                    }
                }

                // Hide the ship label and marker item
                shipLabel.visible = false;
                markerItem.visible = false;

                console.log("Past track and ship markers hidden");
            }

            function markShipOnMap(name, latitude, longitude) {
                console.log(" @@ Marking ship on map:", name, "at", latitude, longitude)

                if (!mapview) {
                    console.error("Map object is not defined")
                    return
                }

                // Remove any existing ship markers with the same name
                for (var i = mapview.mapItems.length - 1; i >= 0; i--) {
                    if (mapview.mapItems[i].objectName === "shipMarker_" + name) {
                        mapview.removeMapItem(mapview.mapItems[i])
                    }
                }

                // Create a new MapQuickItem
                var marker = markerComponent.createObject(mapview, {
                    coordinate: QtPositioning.coordinate(latitude, longitude),
                    objectName: "shipMarker_" + name
                })

                if (marker) {
                    mapview.addMapItem(marker)
                    console.log("Added new marker for", name, "at", latitude, longitude)
                } else {
                    console.error("Failed to create marker for", name)
                }

                // Create a new MapQuickItem for the ship label
                    var label = shipLabelComponent2.createObject(mapview, {
                        coordinate: QtPositioning.coordinate(latitude, longitude),
                        objectName: "shipLabel_" + name
                    })

                    if (label) {
                        label.sourceItem.children[0].text = name
                        mapview.addMapItem(label)
                        console.log("Added new label for", name)
                    } else {
                        console.error("Failed to create label for", name)
                    }

                // Center the map on the ship's position
                mapview.center = QtPositioning.coordinate(latitude, longitude)
                mapview.zoomLevel = 14  // Adjust this value as needed

                console.log("Map centered at", mapview.center.latitude, mapview.center.longitude, "with zoom level", mapview.zoomLevel)
                console.log("Total map items:", mapview.mapItems.length)
            }

            Component {
                id: markerComponent
                MapQuickItem {
                    anchorPoint.x: rect.width/2
                    anchorPoint.y: rect.height/2

                    sourceItem: Rectangle {
                        id: rect
                        width: 20
                        height: 20
                        color: "transparent"  // You can change this color as needed
                        border.color: "blue"
                        border.width: 2
                        radius: width/2  // This makes the rectangle circular

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                console.log("Clicked ship:", parent.parent.objectName)
                                // You can add more functionality here if needed
                            }
                        }


                    }
                }
            }
            // Add this component definition somewhere in your QML file, outside of any function
            Component {
                id: shipLabelComponent2
                MapQuickItem {
                    anchorPoint.x: labelBox.width /2
                    anchorPoint.y: labelBox.height +10

                    sourceItem: Rectangle {
                        id: labelBox
                        width: shipNameText.width + 10
                        height: shipNameText.height + 6
                        color: "white"
                        border.color: "black"
                        border.width: 1
                        radius: 3

                        Text {
                            id: shipNameText
                            anchors.centerIn: parent
                            font.pixelSize: 12
                            font.bold: true
                        }
                }   }
            }
    //MAP starts
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

                // Calculate and update displacement
                var displacement = calculateDistance(
                    coordinate.latitude, coordinate.longitude,
                    fixedMarker.coordinate.latitude, fixedMarker.coordinate.longitude
                )
                bottomBar.updateDisplacement(displacement.toFixed(2))
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
                property color string
                id: shipItem
                property var shipDetails: shipData.getShipDetails(modelData.uuid)
                visible: shipData.isValidShip(modelData.uuid)
                coordinate: QtPositioning.coordinate(shipDetails.latitude, shipDetails.longitude)
                anchorPoint.x: image.width/2
                anchorPoint.y: image.height/2



                sourceItem: Image {

                    // id: image
                    // //source: "qrc:/ship.png"
                    // source: "qrc:/cargo.svg"
                    // width: 30
                    // height: 10
                    // anchors.fill: parent
                    // fillMode: Image.PreserveAspectFit

                    id: container
                                width: getSizeForZoomLevel(mapview.zoomLevel)
                                height: width / 3  // Maintain aspect ratio

                                function getSizeForZoomLevel(zoomLevel) {
                                    // Define sizes for specific zoom levels
                                    if (zoomLevel <= 5) return 10
                                    if (zoomLevel <= 10) return 20
                                    if (zoomLevel <= 15) return 30
                                    if (zoomLevel <= 20) return 40
                                    return 50  // For zoom levels > 20
                                }

                                Image {
                                    id: image
                                    source: "qrc:/cargo.svg"
                                    anchors.fill: parent
                                    fillMode: Image.PreserveAspectFit
                                    rotation: shipDetails.course_over_ground - 90 || 0
                                }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            mapview.children[0].shipClicked = true;  // Set the flag
                            console.log("Clicked MMSI:", shipDetails.mmsi);
                            console.log("Latitude:", shipDetails.latitude);
                            console.log("Longitude:", shipDetails.longitude);

                            if (shipDetails.latitude && shipDetails.longitude) {
                                handleShipClick(shipDetails.latitude, shipDetails.longitude, shipDetails.track_name, shipDetails.mmsi);
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
                            vesselNameText.text = "Vessel Name: " + shipDetails.track_name;
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
                            infoPanel.currentShipUuid = shipUuid
                            infoPanel.visible = true; // Open the drawer when a ship is clicked

                            // Fetch track history
                            //pastTrail.fetchTrackHistory(shipUuid, 48);
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
                color: "#555"  // Background color
                radius: 8  // Rounded corners
                border.color: "#666"  // Subtle border
                border.width: 1
            }
            contentItem: Image {
                source: "qrc:/ship-3.png"
                height: 20
                width:20
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
            }
            onClicked: {
                // Center the map on the fixedMarker's coordinate
                mapview.center = fixedMarker.coordinate
                mapview.zoomLevel = 15 // Optional: Adjust the zoom level as needed
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    // Center the map on the fixedMarker's coordinate
                    mapview.center = fixedMarker.coordinate
                    mapview.zoomLevel = 15 // Optional: Adjust the zoom level as needed
                }
                onPressedChanged: {
                    if (pressed) {
                        parent.background.color = "#444"  // Darker on press
                    } else {
                        parent.background.color = "#555"  // Original color
                    }
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
                color: "#555"  // Background color
                radius: 8  // Rounded corners
                border.color: "#666"  // Subtle border
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
                        parent.background.color = "#444"  // Darker on press
                    } else {
                        parent.background.color = "#555"  // Original color
                    }
                }
            }
        }

        Button {
            width: 33
            height: 33
            background: Rectangle {
                color: "#555"  // Background color
                radius: 8  // Rounded corners
                border.color: "#666"  // Subtle border
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
                        parent.background.color = "#444"  // Darker on press
                    } else {
                        parent.background.color = "#555"  // Original color
                    }
                }
            }
        }

        Button {
            width: 33
            height: 33
            background: Rectangle {
                color: "#555"  // Background color
                radius: 8  // Rounded corners
                border.color: "#666"  // Subtle border
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
                        parent.background.color = "#444"  // Darker on press
                    } else {
                        parent.background.color = "#555"  // Original color
                    }
                }
            }
        }

        Button {
            width: 33
            height: 33
            background: Rectangle {
                color: "#555"  // Background color
                radius: 8  // Rounded corners
                border.color: "#666"  // Subtle border
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
                        parent.background.color = "#444"  // Darker on press
                    } else {
                        parent.background.color = "#555"  // Original color
                    }
                }
            }
        }

        Button {
            width: 33
            height: 33
            background: Rectangle {
                color: "#555"  // Background color
                radius: 8  // Rounded corners
                border.color: "#666"  // Subtle border
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
                        parent.background.color = "#444"  // Darker on press
                    } else {
                        parent.background.color = "#555"  // Original color
                    }
                }
            }
        }

        Button {
            width: 33
            height: 33
            background: Rectangle {
                color: "#555"  // Background color
                radius: 8  // Rounded corners
                border.color: "#666"  // Subtle border
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
                        parent.background.color = "#444"  // Darker on press
                    } else {
                        parent.background.color = "#555"  // Original color
                    }
                }
            }
        }
    }

    // ===================================================================================================================================================================

       function handleShipClick(latitude, longitude, shipName, mmsi) {
        console.log("Received latitude:", latitude, "longitude:", longitude);

        if (latitude && longitude) {
            var coordinate = QtPositioning.coordinate(latitude, longitude);
            console.log("Created coordinate:", coordinate);

            if (mapview) {
                mapview.center = coordinate;
                mapview.zoomLevel = 10;
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

        console.log("Main QML completed. pastHistory is null:", pastHistory === null)
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

        // Calculate and update displacement
            var displacement = calculateDistance(lat, lon, markerLat, markerLon)
            //console.log("Calculated displacement in updatePositionAndBearing:", displacement.toFixed(2))
            bottomBar.currentDisplacement = displacement  // Update this line
    }

    // ... existing code ...


    // ... existing code ...

    BottomBar {
        id: bottomBar

        // Add this property
            property real currentDisplacement: 0

        anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            z: 1 // Ensure it's above the map


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

}
