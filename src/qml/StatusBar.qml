import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

Pane {
    id: statusBar
    objectName: "statusBar"
    height: statusBarLayout.implicitHeight
    padding: 6

    Rectangle {
        parent: statusBar.background
        width: parent.width
        height: 1
        color: "#444"
    }

    Rectangle {
        parent: statusBar.background
        x: parent.width - 1
        width: 1
        height: parent.height
        color: "#444"
    }

    RowLayout {
        id: statusBarLayout
        objectName: "statusBarLayout"
        width: parent.width
        visible: true
        spacing: 0

        FpsCounter {
            id: fpsCounter
            visible: settings.fpsVisible

            Layout.alignment: Qt.AlignVCenter
            Layout.minimumWidth: fpsMaxTextMetrics.width
            Layout.maximumWidth: fpsMaxTextMetrics.width

            TextMetrics {
                id: fpsMaxTextMetrics
                font: fpsCounter.font
                text: "100 FPS"
            }
        }
    }
}
