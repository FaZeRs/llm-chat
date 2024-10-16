import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "./components"

RowLayout {
    id: chatView
    anchors.fill: parent
    anchors.margins: 10
    spacing: 20

    ChatSidebar {
        id: chatSidebar
    }

    Rectangle {
        Layout.preferredWidth: 1
        Layout.fillHeight: true
        color: Material.color(Material.Grey)
    }

    ColumnLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        ChatThread {
            id: chatThread
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        ChatInput {
            id: chatInput
        }
    }
}
