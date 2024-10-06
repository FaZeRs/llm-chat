import QtQuick.Controls

ToolButton {
    id: root
    focusPolicy: Qt.NoFocus
    width: 48
    height: 38
    anchors.verticalCenter: parent.verticalCenter

    ToolTip.visible: hovered
    ToolTip.delay: 800
    ToolTip.timeout: 5000
}
