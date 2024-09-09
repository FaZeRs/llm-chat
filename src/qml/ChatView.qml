import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout {
    id: chatView
    anchors.fill: parent
    anchors.margins: 10

    ListView {
        id: chatList
        Layout.fillWidth: true
        Layout.fillHeight: true
        spacing: 8
        clip: true
        model: chatBackend.thread
        delegate: Rectangle {
            x: model.isUser ? 0 : chatList.width - width
            width: Math.min(messageText.implicitWidth + 24, (chatList.width * 0.8))
            height: messageText.implicitHeight + 20
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: -0.2; color: model.isUser ? "#252B35" : "#32CEF2" }
                GradientStop { position: 1.0; color: model.isUser ? "#3f4045" : "#2D6CF7" }
            }
            radius: 12

            Text {
                id: messageText
                text: model.text
                color: "#FFFFFF"
                wrapMode: Text.Wrap
                anchors.fill: parent
                anchors.margins: 10
                textFormat: Text.MarkdownText
            }
        }
        ScrollBar.vertical: ScrollBar{}
    }

    RowLayout {
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
            onClicked: sendMessage(); 
        }

        Shortcut {
            sequence: visible ? "Ctrl+Return" : ""
            onActivated: sendMessage();
        }
    }

    function sendMessage() {
        if(messageField.text == "") {
            return;
        }

        chatBackend.sendMessage(messageField.text);
        messageField.text = "";
    }
}