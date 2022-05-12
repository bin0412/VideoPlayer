import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Rectangle {
    id: playRec
    color: "black"
    z: 0
    function playVideo(source) {
        video.source = source
        video.play()
    }

    function getTimeString(time) {
        var sec = (time / 1000).toFixed(0);
        let min = (sec / 60).toFixed(0);
        sec = sec % 60;
        let formattedMin = min > 9 ? "" + min : "0" + min
        let formattedSec = sec > 9 ? "" + sec : "0" + sec
        return formattedMin + ":" + formattedSec
    }
    property string playAreaColor : "transparent"
    property bool playState: false
    MouseArea {
        id: play_area
        z: 0
        anchors.fill: playRec
        hoverEnabled: true
        onEntered: {
            playRec.playAreaColor = Qt.rgba(0.1, 0.1, 0.1, 0.2)
        }
        onExited: {
            playRec.playAreaColor = "transparent"
        }
        onClicked: {
            video.playbackState === MediaPlayer.PlayingState ? video.pause() : video.play()
        }
    }

    Video {
        id: video
        z: 0
        anchors.fill: parent
        property string currentTime: "00:00"
        property string totalTime: "00:00"
        onPlaybackStateChanged: {
            play.icon.source = video.playbackState === MediaPlayer.PlayingState ? "qrc:/image/player-pause.png" : "qrc:/image/player-play.png"
        }

        onPositionChanged: {
            progress.value = video.position / video.duration
            currentTime = getTimeString(video.position)
            playbackTimeLabel.text = currentTime + "/" + totalTime
        }

        onDurationChanged: {
            totalTime = getTimeString(video.duration)
            playbackTimeLabel.text = currentTime + "/" + totalTime
        }

        Rectangle {
            id: control
            width: parent.width * 0.7; height: parent.height * 0.1
            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
                bottomMargin: height * 0.5
            }
            radius: height * 0.1
            color: playRec.playAreaColor
            RowLayout {
                id: playerControl
                width: parent.width * 0.4; y: parent.height * 0.05
                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                    id: prev
                    hoverEnabled: false // true导致图标闪现(鼠标事件无法透传到外层MouseArea)
                    onClicked: {
                        video.position -= 500
                    }
                    icon.source: "qrc:/image/player-track-prev.png"
                    Layout.alignment: Qt.AlignLeft
                    focus: true
                    visible: playRec.playAreaColor !== "transparent"
                }
                Button {
                    id: play
                    hoverEnabled: false
                    icon.source: "qrc:/image/player-play.png"
                    onClicked: {
                        video.playbackState === MediaPlayer.PlayingState ? video.pause() : video.play()
                    }
                    Layout.alignment: Qt.AlignCenter
                    visible: playRec.playAreaColor !== "transparent"
                }
                Button {
                    id: next
                    hoverEnabled: false
                    onClicked: {
                        video.position += 500
                    }
                    icon.source: "qrc:/image/player-track-next.png"
                    Layout.alignment: Qt.AlignRight
                    visible: playRec.playAreaColor !== "transparent"
                }
            }
            RowLayout {
                width: control.width * 0.8
                anchors {
                    top: playerControl.bottom
                    horizontalCenter: playerControl.horizontalCenter
                }
                Layout.alignment: Qt.AlignVCenter
                spacing: 5

                ProgressBar {
                    id: progress
                    hoverEnabled: false
                    value: 0
                    Layout.fillWidth: true
                    visible: playRec.playAreaColor !== "transparent"
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: false
                    }
                }

                Label {
                    id: playbackTimeLabel
                    color: "red"
                    text: "00:00/00:00"
                    visible: playRec.playAreaColor !== "transparent"
                }
            }
        }
    }
}
