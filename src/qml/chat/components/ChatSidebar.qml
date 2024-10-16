import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
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
        model: chatBackend.sortedThreads
        delegate: ChatItem {}
        focus: true
        ScrollBar.vertical: ScrollBar {}
        Connections {
            target: chatBackend
            function onCurrentThreadChanged(index) {
                threadList.currentIndex = index;
            }
        }
    }
}
