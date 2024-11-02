import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "." as App

ColumnLayout {
    id: settingsTab

    function applyChangesToSettings() {
        settings.language = languageComboBox.currentValue;
        settings.fpsVisible = showFpsCheckBox.checked;
        settings.windowOpacity = windowOpacitySlider.value;
    }

    function revertToOldSettings() {
        languageComboBox.currentIndex = languageComboBox.indexOfValue(settings.language);
        showFpsCheckBox.checked = settings.fpsVisible;
        windowOpacitySlider.value = settings.windowOpacity;
    }

    Item {
        Layout.preferredHeight: 10
    }

    ScrollView {
        clip: true

        ScrollBar.horizontal.policy: ScrollBar.AsNeeded

        GridLayout {
            columns: 2
            columnSpacing: 12
            width: parent.width

            Label {
                text: qsTr("Language")
            }
            ComboBox {
                id: languageComboBox
                objectName: "languageComboBox"
                textRole: "display"
                valueRole: "value"

                Layout.fillWidth: true

                Component.onCompleted: currentIndex = indexOfValue(settings.language)

                model: [
                    {
                        value: "en_GB",
                        display: "English"
                    }
                ]
            }

            Label {
                text: qsTr("Window opacity")
            }
            Slider {
                id: windowOpacitySlider
                from: 0.5
                value: settings.windowOpacity
                to: 1
                stepSize: 0.05
                objectName: "windowOpacitySlider"

                ToolTip.text: qsTr("Changes the opacity of the window. Useful for tracing over an image in another window.")
                ToolTip.visible: hovered
                ToolTip.delay: 800
                ToolTip.timeout: 5000

                Binding {
                    target: dialog.ApplicationWindow.window
                    property: "opacity"
                    value: windowOpacitySlider.value
                    when: windowOpacitySlider.pressed
                }

                ToolTip {
                    y: -implicitHeight - 12
                    parent: windowOpacitySlider.handle
                    visible: windowOpacitySlider.pressed
                    text: windowOpacitySlider.value.toFixed(1)
                }
            }

            Label {
                text: qsTr("Show FPS")
            }
            CheckBox {
                id: showFpsCheckBox
                checked: settings.fpsVisible
                leftPadding: 0
            }
        }
    }
}
