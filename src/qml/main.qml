import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "." as App
import "./chat" as Chat

ApplicationWindow {
    id: window
    objectName: "window"
    title: qsTr("LLM-Chat")
    visible: true
    width: 1000
    height: 800
    opacity: settings.windowOpacity

    property int oldWindowVisibility: Window.Windowed

    function toggleFullScreen() {
        if (window.visibility === Window.FullScreen) {
            window.visibility = oldWindowVisibility;
        } else {
            oldWindowVisibility = window.visibility;
            window.visibility = Window.FullScreen;
        }
    }

    menuBar: App.MenuBar {
        id: menuBar
        objectName: "menuBar"
        width: parent.width
    }

    header: App.ToolBar {
        id: toolBar
    }

    StackView {
        id: stack
        anchors.left: parent.left
        anchors.top: toolBar.bottom
        anchors.right: parent.right
        anchors.bottom: statusBar.top

        Chat.ChatView {}
    }

    footer: App.StatusBar {
        id: statusBar
        objectName: "statusBar"
        parent: ApplicationWindow.window.contentItem
        width: parent.width
        anchors.bottom: parent.bottom
    }

    App.AboutDialog {
        id: aboutDialog
        anchors.centerIn: parent
        parent: Overlay.overlay
    }

    App.OptionsDialog {
        id: optionsDialog
        anchors.centerIn: parent
        parent: Overlay.overlay
    }
}
