Rectangle {
    id: root
    x: model.isUser ? 0 : chatThread.width - width
    width: Math.min(messageText.implicitWidth + 24, (chatThread.width * 0.8))
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
