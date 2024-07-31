import QtQuick
import QtQuick.Controls

MenuBar {
    spacing: 10
    
    Action {
        id: newAction
        text: qsTr("New")
        shortcut: settings.newShortcut
        onTriggered: console.log("New action triggered");
    }

    Action {
        id: quitAction
        text: qsTr("Exit")
        shortcut: settings.quitShortcut
        onTriggered: close()
    }

    Action {
        id: aboutAction
        text: qsTr("About")
        onTriggered: aboutDialog.open()
    }

    Action {
        id: optionsAction
        text: qsTr("Options")
        shortcut: settings.optionsShortcut
        onTriggered: optionsDialog.open()
    }

    Menu {
        title: qsTr("File")
        MenuItem {
            action: newAction                
        }
        MenuSeparator { }
        MenuItem {
            action: quitAction
        }
    }
    Menu {
        title: qsTr("Tools")
        MenuItem {
            action: optionsAction
        }
    }
    Menu {
        title: qsTr("Help")
        MenuItem {
            action: aboutAction
        }
    }
}