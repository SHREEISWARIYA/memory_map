import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Drawer {
    id: bottomDrawer
    visible: false
    width: parent.width
    height: parent.height * 0.5
    edge: Qt.BottomEdge
    interactive: true

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        // Wrap TableView in a ScrollView
        ScrollView {
            id: scrollView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOn
            ScrollBar.vertical.policy: ScrollBar.AsNeeded

            TableView {
                id: tableView
                width: Math.max(implicitWidth, scrollView.width)

                model: shipDataModel.tableModel

                columnWidthProvider: function(column) { 
                    if (column === 0) return 100;  // MMSI
                    if (column === 1) return 150;  // Track Name
                    if (column === 2 || column === 3) return 100;  // Latitude and Longitude
                    return 120;  // Other columns
                }

                // Header row (outside ScrollView to keep it fixed)
                Row {
                    id: headerRow
                    Layout.fillWidth: true
                    height: 35
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
                    implicitWidth: tableView.columnWidthProvider(column)
                    implicitHeight: 35
                    border.width: 1
                    border.color: "lightgray"

                    Text {
                        anchors.fill: parent
                        anchors.margins: 5
                        text: (display != "Action") ? display : ""
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        color: column === 0 ? "blue" : "black"
                        font.underline: column === 0
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: column === 0 ? Qt.PointingHandCursor : Qt.ArrowCursor
                        onClicked: {
                            if (column === 0) {
                                var shipData = shipDataModel.tableModel.getShipDataForRow(row);
                                console.log("Clicked MMSI:", shipData.mmsi);
                                bottomDrawer.close()
                                console.log("Latitude:", shipData.latitude, "Longitude:", shipData.longitude);
                                handleShipClick(shipData.latitude, shipData.longitude, shipData.track_name, shipData.mmsi);
                            } else {
                                shipLabel.visible = false;
                            }
                        }
                    }
                }
            }
        }



        // Pagination controls (unchanged)
        RowLayout {
            Layout.alignment: Qt.AlignHCenter

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
            Item { Layout.fillWidth: true }
            PaginationControl {
                currentPage: shipDataModel.currentPage
                totalPages: Math.max(1, Math.ceil(shipDataModel.totalShips / shipDataModel.itemsPerPage))
                onPageChanged: shipDataModel.currentPage = page
            }
            Item { Layout.fillWidth: true }
            RowLayout {
                Label { text: "Items per page:" }
                ComboBox {
                    id: itemsPerPageComboBox
                    model: [5, 10, 15, 20, 25, 30]
                    currentIndex: model.indexOf(shipDataModel.itemsPerPage)
                    onCurrentIndexChanged: {
                        shipDataModel.itemsPerPage = model[currentIndex];
                    }
                }
            }
        }
    }
}









// import QtQuick 2.15
// import QtQuick.Controls 2.15
// import QtQuick.Layouts 1.15


// Drawer {
//     id: bottomDrawer
//     visible: false
//     width: parent.width
//     height: parent.height * 0.5 // Set the height to 50% of the parent window
//     edge: Qt.BottomEdge
//     interactive: true

//     // Remove the Shortcut and toggleFullScreen function as they're not needed for a drawer

//     ColumnLayout {
//         anchors.fill: parent
//         spacing: 10


//         TableView {
//             id: tableView
//             Layout.fillWidth: true
//             Layout.fillHeight: true
//             clip: true

//             model: shipDataModel.tableModel

//             columnWidthProvider: function(column) { return 150; }

//             // Header row
//             Row {
//                 id: headerRow
//                 y: tableView.contentY
//                 z: 2
//                 Repeater {
//                     model: tableView.columns
//                     Rectangle {
//                         width: tableView.columnWidthProvider(modelData)
//                         height: 35
//                         color: "lightgray"
//                         border.width: 1
//                         border.color: "gray"

//                         Text {
//                             anchors.centerIn: parent
//                             text: shipDataModel.tableModel.headerData(modelData, Qt.Horizontal)
//                             font.bold: true
//                         }
//                     }
//                 }
//             }

//             // Data rows
//             delegate: Rectangle {
//                 implicitWidth: 150
//                 implicitHeight: 35
//                 border.width: 1
//                 border.color: "lightgray"

//                 Text {
//                     anchors.fill: parent
//                     anchors.margins: 5
//                     text: (display != "Action") ? display : ""
//                     verticalAlignment: Text.AlignVCenter
//                     elide: Text.ElideRight
//                     color: column === 0 ? "blue" : "black" // Make MMSI blue to look like a hyperlink
//                     font.underline: column === 0 // Underline the MMSI
//                 }

//                 // MouseArea {
//                 //     anchors.fill: parent
//                 //     cursorShape: column === 0 ? Qt.PointingHandCursor : Qt.ArrowCursor
//                 //     onClicked: {
//                 //         if (column === 0) { // Check if it's the MMSI column
//                 //             var shipData = shipDataModel.tableModel.getShipDataForRow(row);
//                 //             console.log("Clicked MMSI:", shipData.mmsi);
//                 //             bottomDrawer.close()

//                 //             console.log("Latitude:", shipData.latitude, "Longitude:", shipData.longitude);
//                 //             handleShipClick(shipData.latitude, shipData.longitude, shipData.track_name, shipData.mmsi);
//                 //         } else {
//                 //             // Hide the ship name label when clicking elsewhere
//                 //             shipLabel.visible = false;
//                 //         }
//                 //     }
//                 // }

//                 MouseArea {
//                         anchors.fill: parent
//                         cursorShape: column === 0 ? Qt.PointingHandCursor : Qt.ArrowCursor
//                         onClicked: {
//                             if (column === 0) { // Check if it's the MMSI column
//                                 var shipData = shipDataModel.tableModel.getShipDataForRow(row);
//                                 console.log("Clicked MMSI:", shipData.mmsi);
//                                 bottomDrawer.close()

//                                 console.log("Latitude:", shipData.latitude, "Longitude:", shipData.longitude);
//                                 handleShipClick(shipData.latitude, shipData.longitude, shipData.track_name, shipData.mmsi);
//                             } else {
//                                 // Hide the ship name label when clicking elsewhere
//                                 shipLabel.visible = false;
//                             }
//                         }
//                 }
//             }

//             // Adjust the top margin to account for the header row
//             topMargin: headerRow.height
//         }

//         // Pagination controls
//         RowLayout {
//             Layout.alignment: Qt.AlignHCenter

//             // Entries info text
//             Text {
//                 id: entriesInfoText
//                 Layout.alignment: Qt.AlignHCenter

//                 text: {
//                     if (shipDataModel.totalShips > 0) {
//                         var start = (shipDataModel.currentPage - 1) * shipDataModel.itemsPerPage + 1;
//                         var end = Math.min(start + shipDataModel.itemsPerPage - 1, shipDataModel.totalShips);
//                         return "Showing " + start + " to " + end + " of " + shipDataModel.totalShips + " entries";
//                     } else {
//                         return "Showing 0 to 0 of 0 entries";
//                     }
//                 }
//             }
//             Item {
//                 Layout.fillWidth: true // This will take up all available space
//             }
//             PaginationControl {
//                 currentPage: shipDataModel.currentPage
//                 totalPages: Math.max(1, Math.ceil(shipDataModel.totalShips / shipDataModel.itemsPerPage))
//                 onPageChanged: shipDataModel.currentPage = page
//             }
//             Item {
//                 Layout.fillWidth: true // This will take up all available space
//             }
//             RowLayout {
//                 Label { text: "Items per page:" }

//                 ComboBox {
//                     id: itemsPerPageComboBox
//                     model: [5, 10, 15, 20, 25, 30] // Options for items per page
//                     currentIndex: model.indexOf(shipDataModel.itemsPerPage)
//                     onCurrentIndexChanged: {
//                         shipDataModel.itemsPerPage = model[currentIndex];
//                     }
//                 }
//             }
//         }
//     }
// }

// // ... existing code ...
