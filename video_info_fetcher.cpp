#include "video_info_fetcher.h"
#include <QtMultimedia/QMediaPlayer>
#include <QVideoSink>
#include <QtMultimedia/QVideoFrame>
#include <QThread>

QMediaPlayer* sharedPlayer() {
    static auto g_player = new QMediaPlayer();
    return g_player;
}

bool fetchFirstFrameFromVideoUrl(const QString& url, std::function<void(const QImage&)> callback)
{
    sharedPlayer()->setSource(QUrl(url));
    auto sink = new QVideoSink;
    sharedPlayer()->setVideoSink(sink);
    qDebug() << "[debug] : 调用fetchFirstFrameFromVideoUrl()函数...";
    QObject::connect(sink, &QVideoSink::videoFrameChanged, sharedPlayer(), [=] (const QVideoFrame &frame) {
           if (frame.isValid()) {
               sharedPlayer()->stop();
               qDebug() << "调用lambda";
               static int flag = 0; // 暂时通过flag来控制视频重复添加问题
               if((flag % 3) == 0) {
                   callback(frame.toImage());
                }
               ++flag;
           }

    }, Qt::QueuedConnection);
    sharedPlayer()->play();
    return true;
}
