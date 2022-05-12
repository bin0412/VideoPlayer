/**************************************************************************
*   @fileName      video_info_fetcher.h
*   @projectName   VideoPlayerDemo
*   @date          2022-03-07
*   @author        Admin
*   @brief
*   @copyright     Copyright 漏 2022 Bytedance All rights reserved.
**************************************************************************/
#ifndef VIDEO_INFO_FETCHER_H
#define VIDEO_INFO_FETCHER_H
#include <QImage>

bool fetchFirstFrameFromVideoUrl(const QString& url, std::function<void(const QImage&)> callback);

#endif // VIDEO_INFO_FETCHER_H
