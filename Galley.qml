import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import com.bytedance.asset

Rectangle {
    id: root
    color: "transparent"

    property VideoAssetModel videoModel: VideoAssetModel{}
    signal selectedVideo(string url) // 选择视频时触发

    GridView {
        id: grid
        anchors.fill: parent
        cellWidth: 120; cellHeight: 100
        focus: true
        model: videoModel
        currentIndex: -1
        property int index: -1 // 初始时不做选择

        highlight: Rectangle {
            width: 80
            height: 80
            color: "gray"
        }

        delegate: Item {
            width: grid.cellWidth;
            height: grid.cellHeight
            Image {
                width: grid.cellWidth * 0.9
                height: grid.cellHeight * 0.6
                id: coverImage
                y: grid.cellHeight * 0.1
                anchors.horizontalCenter: parent.horizontalCenter
                source: asset.coverImage
            }
            Item {
                id: text_area
                width: grid.cellWidth * 0.88
                anchors.top: coverImage.bottom
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                clip: true
                Text {
                    id: title
                    width:  title.implicitWidth // 注意这样才可以完整完成滚动动画效果
                    text: asset.name
                    color: "white"
                    clip: true
                    elide: Text.ElideNone
                    horizontalAlignment: Text.AlignHCenter | Text.AlignVCenter

                    SequentialAnimation on x {
                        loops: Animation.Infinite
                        running: grid.currentIndex === model.index
                        PropertyAnimation {
                            from: 0
                            to: -title.width
                            duration: 8000
                        }
                    }
                }

            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: (event) => {
                    if (event.button === Qt.LeftButton) {
                        grid.currentIndex = model.index
                        parent.GridView.view.currentIndex = model.index
                        root.selectedVideo(asset.url) // 调用selectedVideo方法，传入视频url
                    }
                    else if (event.button === Qt.RightButton) {
                        grid.model.removeVideo(model.index)
                    }
                }
            }
        }

        Component.onCompleted: model.loadAssets()
    }

    DropArea {
        anchors.fill: parent
        onDropped: (drop) => {
            console.log("onDropped被调用了...")
            drop.acceptProposedAction()
            dropFile(drop)

        }
    }

    function dropFile(file) {
        videoModel.addVideo(file.text);
        console.log("dropFile函数被调用了...")
    }
}
