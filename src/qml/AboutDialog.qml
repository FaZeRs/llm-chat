import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root
    objectName: "aboutDialog"
    title: qsTr("About")
    modal: true
    dim: false
    focus: true
    standardButtons: Dialog.Ok

    ColumnLayout {
        spacing: 12

        Label {
            text: qsTr("Room Sketcher - Floor Plan Creator")
        }

        Label {
            text: qsTr("Version : %1").arg(Qt.application.version)
        }

        Label {
            text: qsTr("Copyright 2023, Giraffe360")
        }
    }
}