/**************************************************************************
*   @fileName      video_asset.h
*   @projectName   VideoPlayerDemo
*   @date          2022-03-07
*   @author        Admin
*   @brief
*   @copyright     Copyright 漏 2022 Bytedance All rights reserved.
**************************************************************************/
#ifndef VIDEO_ASSET_H
#define VIDEO_ASSET_H

#include <QObject>
#include <QAbstractListModel>
#include <QQmlEngine>
#include <QDataStream>

class VideoAsset : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString url READ url CONSTANT)
    Q_PROPERTY(QString coverImage READ coverImage CONSTANT)
public:
    VideoAsset() {}
    VideoAsset(const QString &name, const QString &url, const QString &coverImage, QObject *parent = nullptr) :
        m_name(name), m_url(url), m_coverImage(coverImage){}

    const QString &name() const;
    const QString &url() const;
    const QString &coverImage() const;

    QDataStream &read(QDataStream &in) {
        in >> m_name >> m_url >> m_coverImage;
        return in;
    }
    QDataStream &write(QDataStream &out) const {
        out << m_name << m_url << m_coverImage;
        return out;
    }

private:
    QString m_name;
    QString m_url;
    QString m_coverImage;
};

class VideoAssetModel: public QAbstractListModel
{
    Q_OBJECT
public:
    Q_INVOKABLE void addVideo(const QString &url);
    Q_INVOKABLE void removeVideo(int index);
    Q_INVOKABLE void loadAssets();
private:
    QList<QSharedPointer<VideoAsset>> m_datas;\
    void storeAssets();

public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
};

void registerToQML(QQmlEngine &engine);

#endif // VIDEO_ASSET_H
