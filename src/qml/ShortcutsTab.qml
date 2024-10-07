import QtQml.Models
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout {
    id: shortcutsTab

    function applyChangesToSettings() {
        for (var i = 0; i < shortcutModel.count; ++i) {
            var row = shortcutModel.get(i);
            if (row.hasChanged) {
                settings[row.shortcutName] = row.newSequence;
            }
        }
    }

    function revertToOldSettings() {
        for (var i = 0; i < shortcutModel.count; ++i) {
            var row = shortcutModel.get(i);
            if (row.hasChanged) {
                row.reset();
            }
        }
    }

    Item {
        Layout.preferredHeight: 10
    }

    ScrollView {
        objectName: "behaviourScrollView"
        clip: true

        ScrollBar.horizontal.policy: ScrollBar.AsNeeded

        Layout.fillWidth: true
        Layout.fillHeight: true

        GridLayout {
            columns: 2
            columnSpacing: 12
            width: parent.width

            Repeater {
                model: ObjectModel {
                    id: shortcutModel

                    ShortcutRow {
                        shortcutName: "quitShortcut"
                        shortcutDisplayName: qsTr("Quit")
                    }
                    ShortcutRow {
                        shortcutName: "optionsShortcut"
                        shortcutDisplayName: qsTr("Options")
                    }
                    ShortcutRow {
                        shortcutName: "fullScreenShortcut"
                        shortcutDisplayName: qsTr("Toggle fullscreen")
                    }
                }
            }
        }
    }
}
