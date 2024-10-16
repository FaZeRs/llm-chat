import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    id: root

    TextField {
        id: messageField
        placeholderText: qsTr("Type your message here...")
        Layout.fillWidth: true
        onAccepted: sendMessage()
    }

    RoundButton {
        id: sendButton
        text: "\uf054"
        font.family: "Font Awesome 6 Free Solid"
        onClicked: sendMessage()
    }

    Shortcut {
        sequence: visible ? "Ctrl+Return" : ""
        onActivated: sendMessage()
    }

    function sendMessage() {
        if (messageField.text == "") {
            return;
        }
        chatBackend.sendMessage(threadList.currentIndex, messageField.text);
        messageField.text = "";
    }
}
