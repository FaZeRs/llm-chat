import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
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
                chatBackend.deleteThread(index);
            }
        }
    }
}
