// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0

Rectangle {
    color: delegateMouseArea.pressed ? "grey" : "white"
    Row {
        anchors {
            fill: parent
            margins: 8
        }
        spacing: 4

        Image {
            id: documentTypeIcon
            height: parent.height
            width: height
            source: "qrc:///doc_icons/" + type + ".png"
        }

        Label {
            height: parent.height
            width: parent.width - documentTypeIcon.width
            text: display
            elide: Text.ElideRight
            maximumLineCount: 1
            font: UiConstants.SmallTitleFont
        }
    }

    MouseArea {
        id: delegateMouseArea
        anchors.fill: parent
        onClicked: gDocumentService.downloadDocument(url, type)
    }
}
