import QtQuick
import QtQuick.Controls

ListView {
    id: root
    spacing: 8
    clip: true
    model: chatBackend.thread(threadList.currentIndex)
    delegate: Rectangle {
        x: model.isUser ? 0 : chatList.width - width
        width: Math.min(messageText.implicitWidth + 24, (chatList.width * 0.8))
        height: messageText.implicitHeight + 20
        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop {
                position: -0.2
                color: model.isUser ? "#252B35" : "#32CEF2"
            }
            GradientStop {
                position: 1.0
                color: model.isUser ? "#3f4045" : "#2D6CF7"
            }
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
    ScrollBar.vertical: ScrollBar {}
}
