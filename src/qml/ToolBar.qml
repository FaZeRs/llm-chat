import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Templates as T
import QtQuick.Window

import "." as App

ToolBar {
    id: root
    objectName: "toolBar"

    Row {
        id: toolbarRow
        anchors.fill: parent

        App.ToolButton {
            id: fullScreenToolButton
            objectName: "fullScreenToolButton"
            text: "\uf065"
            font.family: "Font Awesome 6 Free Solid"
            font.weight: 900
            font.pixelSize: 18
            checkable: true
            checked: window.visibility === Window.FullScreen

            ToolTip.text: qsTr("Toggle fullscreen window")

            onClicked: toggleFullScreen()
        }

        App.ToolButton {
            id: settingsToolButton
            objectName: "settingsToolButton"
            text: "\uf013"
            font.family: "Font Awesome 6 Free Solid"
            font.weight: 900
            font.pixelSize: 18
            ToolTip.text: qsTr("Settings")

            onClicked: optionsDialog.open()
        }
    }
}
