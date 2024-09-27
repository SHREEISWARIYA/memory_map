import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

RowLayout {
    property int currentPage: 1
    property int totalPages: 1
    property int visiblePages: 5

    signal pageChanged(int page)

    function updateButtons() {
        var start = Math.max(1, Math.min(currentPage - Math.floor(visiblePages / 2), totalPages - visiblePages + 1));
        var end = Math.min(start + visiblePages - 1, totalPages);
        start = Math.max(1, end - visiblePages + 1);

        for (var i = 0; i < visiblePages; i++) {
            var pageNumber = start + i;
            pageButtons.itemAt(i).text = pageNumber;
            pageButtons.itemAt(i).visible = pageNumber <= totalPages;
        }
    }

    Button {
        Image {
                    source: "qrc:/previous_button.png"  // Replace with your image path
                    anchors.centerIn: parent
                    width: 20  // Adjust as needed
                    height: 20 // Adjust as needed

        }
        opacity: enabled ? 1.0 : 0.5
        onClicked: if (currentPage > 1) pageChanged(currentPage - 1)
        enabled: currentPage > 1
    }

    Repeater {
        id: pageButtons
        model: visiblePages
        Button {
            text: index + 1
            onClicked: pageChanged(Number(text))
            highlighted: Number(text) === currentPage
        }
    }

    Button {
        Image {
            source: "qrc:/next_button.png"  // Replace with your image path
            anchors.centerIn: parent
            width: 20  // Adjust as needed
            height: 20 // Adjust as needed
        }
        opacity: enabled ? 1.0 : 0.5
        onClicked: if (currentPage < totalPages) pageChanged(currentPage + 1)
        enabled: currentPage < totalPages
    }

    onCurrentPageChanged: updateButtons()
    onTotalPagesChanged: updateButtons()
    Component.onCompleted: updateButtons()
}
