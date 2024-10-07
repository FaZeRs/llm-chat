import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Label {
    id: root
    objectName: "fpsCounter"
    text: qsTr("%1 FPS").arg(root.fps)

    property int fps: 0

    Item {
        id: rotationItem
        objectName: "fpsCounterRotationItem"

        property int timesUpdated: 0

        NumberAnimation on rotation {
            from: 0
            to: 360
            duration: 500
            loops: Animation.Infinite
            running: root.visible
        }

        onRotationChanged: ++timesUpdated
    }

    Timer {
        interval: 1000
        repeat: true
        running: root.visible
        onTriggered: {
            root.fps = rotationItem.timesUpdated / (interval / 1000);
            rotationItem.timesUpdated = 0;
        }
    }
}
