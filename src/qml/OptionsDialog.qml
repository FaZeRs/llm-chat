import QtQml.Models
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Dialog {
    id: dialog
    objectName: "optionsDialog"
    modal: true
    focus: true
    implicitWidth: 500
    implicitHeight: 400

    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: applyChangesToSettings()
    // We used to use onRejected here, but if the settings changes outside of our control
    // (e.g. through being reset during testing) then some controls will contain outdated
    // values since clearChanges() won't be called in that case.
    onAboutToShow: revertToOldSettings()

    function applyChangesToSettings() {
        settingsTab.applyChangesToSettings();
        shortcutsTab.applyChangesToSettings();
    }

    function revertToOldSettings() {
        settingsTab.revertToOldSettings();
        shortcutsTab.revertToOldSettings();
    }

    header: TabBar {
        id: tabBar

        // For the Universal style.
        // clip: true

        TabButton {
            objectName: "settingsTabButton"
            text: qsTr("General")
        }

        TabButton {
            objectName: "shortcutsTabButton"
            text: qsTr("Shortcuts")
        }
    }

    StackLayout {
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        SettingsTab {
            id: settingsTab
        }

        ShortcutsTab {
            id: shortcutsTab
        }
    }
}
