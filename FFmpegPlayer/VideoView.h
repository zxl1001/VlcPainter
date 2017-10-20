/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    VideoView.h
 * @brief
 *******************************************************************************
 */
#ifndef VIDEOVIEW_H
#define VIDEOVIEW_H

#include "VideoScene.h"
#include <QGraphicsView>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QMutex>

#include "VideoDecoder.h"

namespace Video {

class VideoView;
class VideoScene;

class VideoView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit VideoView(QWidget *parent=0);
    virtual ~VideoView();

    void setEditMode(bool isEditMode);
    bool isEditMode() const;

    void setVideoBufferInterval(ulong bufferInterval);
    void setVideoMaxBufferCount(int maxCount);
    ulong getVideoBufferInterval() const;
    int getVideoMaxBufferCount() const;

    bool initializeVideo(const QString& filePath);
    void uninitializeVideo();

    bool isVideoAvailable() const;
    QString getVideoFilePath() const;
    qint64 getVideoDuration() const;
    float getFrameRate() const;
    int getFrameCount() const;

    void play();
    void pause();
    void stop();
    void gotoFrame(int frameIndex);
    void gotoNextFrame();
    void gotoPrevFrame();
    void gotoTime(qint64 ms);
    void gotoPercent(float percent);

    bool saveCurrentFrame(const QString &path) const;

    bool isReady() const;
    bool isPlaying() const;
    bool isPaused() const;
    int getCurrentFrame() const;
    int getGotoFrame() const;
    qint64 getCurrentTime() const;
    float getCurrentPercent() const;

    VideoScene *getScene() const;

Q_SIGNALS:
    void videoSizeChanged();
    void videoStateChanged(VideoState state);
    void videoFrameChanged(int frameIndex);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onVideoStateChanged(VideoState state);
    void onVideoFrameChanged(int index);

private:
    QString m_mediaFile; //media file
    VideoScene *m_scene;
    bool m_isEditMode;
    VideoDecoder mVideoDecoder;
};

}
#endif // VIDEOVIEW_H
