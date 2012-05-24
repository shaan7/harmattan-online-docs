// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0

Rectangle {
    id: rootRect
    property string text: qsTr("Online Docs")

    color: "orange"
    Label {
        anchors {
            fill: parent
            topMargin: if (screen.currentOrientation === Screen.Portrait)
                           UiConstants.HeaderDefaultTopSpacingPortrait
                       else if (screen.currentOrientation === Screen.Landscape)
                           UiConstants.HeaderDefaultTopSpacingLandscape
            bottomMargin: if (screen.currentOrientation === Screen.Portrait)
                              UiConstants.HeaderDefaultBottomSpacingPortrait
                          else if (screen.currentOrientation === Screen.Landscape)
                              UiConstants.HeaderDefaultBottomSpacingLandscape
            leftMargin: UiConstants.DefaultMargin
            rightMargin: UiConstants.DefaultMargin
        }

        text: rootRect.text
        font: UiConstants.HeaderFont
    }
}
