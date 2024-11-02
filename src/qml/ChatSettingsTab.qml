import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "." as App

ColumnLayout {
    id: chatSettingsTab

    function applyChangesToSettings() {
        chat.setModel(modelComboBox.currentText);
        chat.setSystemPrompt(systemPromptTextField.text);
        chat.setOllamaServerUrl(ollamaServerUrlTextField.text);
    }

    function revertToOldSettings() {
        modelComboBox.currentIndex = modelComboBox.indexOfValue(chat.model);
        systemPromptTextField.text = chat.systemPrompt;
        ollamaServerUrlTextField.text = chat.ollamaServerUrl;
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
            rowSpacing: 12

            Label {
                text: qsTr("Ollama server URL")
            }
            TextField {
                id: ollamaServerUrlTextField
                text: chat.ollamaServerUrl
            }

            Label {
                text: qsTr("Model")
            }
            ComboBox {
                id: modelComboBox

                model: chat.modelList
            }

            Connections {
                target: chat
                function onModelListFetched() {
                    modelComboBox.currentIndex = modelComboBox.indexOfValue(chat.model);
                }
            }

            Label {
                text: qsTr("System prompt")
            }
            TextArea {
                id: systemPromptTextField
                placeholderText: qsTr("Enter a system prompt")
                text: chat.systemPrompt
            }
        }
    }
}
