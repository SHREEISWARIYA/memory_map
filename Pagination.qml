import QtQuick
import QtQuick.Controls

Item {
    width: 640
    height: 480

    Button {
        anchors.centerIn: parent
        text: "Print Pagination Data"
        onClicked: {
            console.log("Button clicked")  // Add this line for debugging
            printPaginationData()
        }
    }

    function printPaginationData() {
        console.log("Printing pagination data...")  // Add this line for debugging
        let ships = pagination.ships
        if (!ships) {
            console.log("Error: pagination.ships is undefined or null")
            return
        }
        //console.log("Number of ships:", Object.keys(ships).length)
        for (let uuid in ships) {
            console.log("UUID:", uuid)
            console.log(JSON.stringify(ships[uuid], null, 2))
            console.log("---")
        }
    }
}
