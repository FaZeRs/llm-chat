import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    id: chatView
    anchors.fill: parent
    anchors.margins: 10
    spacing: 20

    ColumnLayout {
        id: chatSidebar
        Layout.fillHeight: true
        Layout.maximumWidth: chatView.width * 0.2
        Layout.alignment: Qt.AlignTop

        Button {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            text: qsTr("New chat")
            onClicked: {
                threadList.currentIndex = -1;
            }
        }

        ListView {
            id: threadList
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 8
            clip: true
            model: chat.sortedThreads
            delegate: Rectangle {
                property bool active: index == threadList.currentIndex
                width: threadList.width
                height: threadTitle.implicitHeight + 10
                radius: 12
                color: mouse.hovered ? "#292929" : active ? "#424242" : "#303030"
                HoverHandler {
                    id: mouse
                    acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
                    cursorShape: Qt.PointingHandCursor
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: threadList.currentIndex = index
                }
                RowLayout {
                    anchors.fill: parent
                    Text {
                        id: threadTitle
                        text: model.text
                        color: "#FFFFFF"
                        wrapMode: Text.Wrap
                        verticalAlignment: Text.AlignVCenter
                        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.leftMargin: 10
                    }
                    ToolButton {
                        id: deleteThreadToolButton
                        focusPolicy: Qt.NoFocus
                        visible: mouse.hovered

                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        Layout.fillHeight: true
                        Layout.rightMargin: 10

                        text: "\uf1f8"
                        font.family: "Font Awesome 6 Free Solid"
                        font.weight: 900
                        font.pixelSize: 12

                        ToolTip.visible: hovered
                        ToolTip.delay: 800
                        ToolTip.timeout: 5000
                        ToolTip.text: qsTr("Delete chat")

                        onClicked: {
                            if (index == threadList.currentIndex) {
                                threadList.currentIndex = -1;
                            }
                            chat.deleteThread(index);
                        }
                    }
                }
            }
            focus: true
            ScrollBar.vertical: ScrollBar {}
            Connections {
                target: chat
                function onNewThreadCreated() {
                    threadList.currentIndex = 0;
                }
            }
        }

        Button {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            text: qsTr("Clear all chats")
            onClicked: {
                chat.clearThreads();
                threadList.currentIndex = -1;
            }
        }
    }

    Rectangle {
        Layout.preferredWidth: 1
        Layout.fillHeight: true
        color: Material.color(Material.Grey)
    }

    ColumnLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true

        ListView {
            id: chatThread
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 8
            clip: true
            model: chat.getThread(threadList.currentIndex)
            delegate: Rectangle {
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

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 5

                    Text {
                        id: messageText
                        text: model.text
                        color: "#FFFFFF"
                        wrapMode: Text.Wrap
                        Layout.fillWidth: true
                        textFormat: Text.MarkdownText
                    }

                    RowLayout {
                        visible: !model.isUser
                        Layout.alignment: Qt.AlignRight
                        spacing: 5

                        ToolButton {
                            text: "\uf01e"
                            font.family: "Font Awesome 6 Free Solid"
                            font.pixelSize: 12
                            font.weight: 900
                            focusPolicy: Qt.NoFocus
                            visible: index === chatThread.count - 1

                            ToolTip.visible: hovered
                            ToolTip.delay: 800
                            ToolTip.timeout: 5000
                            ToolTip.text: qsTr("Retry last message")

                            onClicked: chat.retryLatestMessage(threadList.currentIndex)
                        }
                    }
                }
            }
            ScrollBar.vertical: ScrollBar {}
        }

        RowLayout {
            id: chatInput

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
                font.weight: 900
                font.pixelSize: 18
                onClicked: sendMessage()
            }

            Shortcut {
                sequence: visible ? "Ctrl+Return" : ""
                onActivated: sendMessage()
            }
        }
    }

    function sendMessage() {
        if (messageField.text == "") {
            return;
        }
        chat.sendMessage(threadList.currentIndex, messageField.text);
        messageField.text = "";
    }
}
