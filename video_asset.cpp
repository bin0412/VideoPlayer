/**************************************************************************
*   @fileName      video_asset.cpp
*   @projectName   VideoPlayerDemo
*   @date          2022-03-07
*   @author        Admin
*   @brief
*   @copyright     Copyright 漏 2022 Bytedance All rights reserved.
**************************************************************************/
#include "video_asset.h"
#include "video_info_fetcher.h"
#include <QQmlEngine>
#include <QQuickImageProvider>
#include <QFile>
#include <QFileInfo>

QString g_image_provider_name = "cover_image";
QString g_image_name_prefix = "image://" + g_image_provider_name + "/";

class CoverImageProvider : public QQuickImageProvider
{
public:
    CoverImageProvider(): QQuickImageProvider(QQuickImageProvider::Pixmap) {}

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override
    {
       return QPixmap(id);
    }
};


const QString & VideoAsset::name() const{
    return m_name;
}
const QString &VideoAsset::url() const{
    return m_url;
}
const QString &VideoAsset::coverImage() const{
    return m_coverImage;
}

void VideoAssetModel::addVideo(const QString &url)
{
    QStringList parts = url.split("/");
    QString filename = parts.at(parts.size()-1);
    fetchFirstFrameFromVideoUrl(url, [=](const QImage& image) {
        QString imageName = filename.split(".").at(0) + ".png";
        QFileInfo info(imageName); int i = 1;
        while (info.exists()) {
            imageName = filename.split(".").at(0) + QString::number(i++) + ".png";
            info = QFileInfo(imageName);
        }
        bool result = image.save(imageName, "PNG");
        auto rowcnt = m_datas.count();
        beginInsertRows(QModelIndex(), rowcnt, rowcnt);
        auto asset = QSharedPointer<VideoAsset>(new VideoAsset(filename, url, g_image_name_prefix + imageName));
        m_datas.append(asset);
        endInsertRows();
        storeAssets();
    });
}

void VideoAssetModel::removeVideo(int index)
{
    if (index < m_datas.count()) {
        beginRemoveRows(QModelIndex(), index, index);
        auto asset = m_datas[index];
        m_datas.remove(index);
        endRemoveRows();
        QString imageName = asset->coverImage();
        imageName = imageName.remove(g_image_name_prefix);
        QFile file(imageName);
        if (file.exists()) {
            file.remove();
        }
        storeAssets();
    }
}

void VideoAssetModel::loadAssets()
{
    QFile file("playlist.dat");
    if (file.open(QIODevice::ReadOnly | QIODevice::ExistingOnly)) {
        QDataStream in(&file);
        int num;
        in >> num;
        beginInsertRows(QModelIndex(), 0, num - 1);
        for (int i = 0; i< num; i++) {
            auto asset =  QSharedPointer<VideoAsset>(new VideoAsset);
            asset->read(in);
            m_datas.append(asset);
        }
        endInsertRows();
    }
}

void VideoAssetModel::storeAssets() {
    QFile file("playlist.dat");
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        int num = m_datas.count();
        out << num;
        for (int i = 0; i< num; i++) {
           m_datas[i]->write(out);
        }
    }
}

int VideoAssetModel::rowCount(const QModelIndex &parent) const {
    return m_datas.count();
}

QVariant VideoAssetModel::data(const QModelIndex &index, int role) const {
    Q_UNUSED(role)
    return QVariant::fromValue(m_datas[index.row()].data());
}

QHash<int, QByteArray> VideoAssetModel::roleNames() const {
    QHash<int, QByteArray> hash;
    hash[0] = "asset";
    return hash;
}

void registerToQML(QQmlEngine &engine)
{
    qmlRegisterType<VideoAssetModel>("com.bytedance.asset", 1, 0, "VideoAssetModel");
    engine.addImageProvider(g_image_provider_name.toLatin1(), new CoverImageProvider);
}
