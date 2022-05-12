import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12

Window {
    width: 1280
    height: 720
    visible: true
    title: qsTr("player")

    RowLayout {
        anchors.fill: parent
        spacing: 0

        VideoPlayback {
            id: video
            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: parent.width * 0.8
            Layout.fillHeight: true
        }

        Galley {
            id: tools
            Layout.alignment: Qt.AlignRight
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
    Component.onCompleted: {
        tools.selectedVideo.connect(video.playVideo)
    }
}
