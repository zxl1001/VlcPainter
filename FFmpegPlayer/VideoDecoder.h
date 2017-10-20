/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    VideoDecoder.h
 * @brief   Video interface encapsulated from FFMpeg library
 *******************************************************************************
 */

#ifndef VIDEODECODER_H
#define VIDEODECODER_H

#include <QImage>
#include <QList>
#include <QThread>
#include <QTimer>
#include <QReadWriteLock>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/mem.h"
#include "libswscale/swscale.h"
}

namespace Video
{

enum class VideoState : int
{
    Ready,
    Playing,
    Paused,

    Invalid = -1
};

struct VideoInfo
{
    QString filePath;

    AVFormatContext* avFormatContext;

    int avStreamIndex;
    float deltaTimeStamp;
    float frameRate;
    AVStream* avStream;

    AVCodecContext* avCodecContext;
    AVCodec* avCodec;

    SwsContext* swsContext;

    VideoInfo();
};

struct VideoFramePacket
{
    AVPacket avPacket;
    AVFrame* avSrcFrame;
    AVFrame* avDstFrame;

    VideoFramePacket();
};

struct VideoFrameData
{
    int index;
    int64_t timestamp;
    QImage image;

    VideoFrameData();
};

class VideoDecoder : public QThread
{
    Q_OBJECT

public:
    VideoDecoder();
    ~VideoDecoder();

    void setBufferInterval(ulong bufferInterval);
    void setMaxBufferCount(int maxCount);
    ulong getBufferInterval() const;
    int getMaxBufferCount() const;

    bool initialize(const QString& filePath);
    void uninitialize();

    bool isVideoAvailable() const;
    const QString& getVideoFilePath() const;
    qint64 getVideoDuration() const;
    float getFrameRate() const;
    int getFrameCount() const;

    bool play();
    bool pause();
    bool stop();
    bool gotoFrame(int frameIndex);
    bool gotoTime(qint64 ms);
    bool gotoPercent(float percent);

    VideoState getState() const;
    bool isReady() const;
    bool isPlaying() const;
    bool isPaused() const;
    int getCurrentFrame() const;
    int getGotoFrame() const;
    qint64 getCurrentTime() const;
    float getCurrentPercent() const;

    bool hasFrameData(int index) const;
    bool getFrameData(int index, VideoFrameData& frameData, bool matchIndex = false) const;

    bool saveCurrentFrame(const QString& path) const;

signals:
    void stateChanged(VideoState state);
    void frameChanged(int index);

protected:
    void run() override;

private slots:
    void onPlayTimer();

private:
    bool addFrameData(const VideoFrameData& frameData, bool checkSequence);
    void clearFramesOutOfBuffer();
    void clearAllFrames();

    bool gotoFrameInternal(int frameIndex);
    bool gotoTimeStamp(int64_t pts);
    bool gotoTimeStampBackward(int64_t pts, VideoFrameData& frameData);
    bool seekToTimeStampOnce(int64_t pts);

    bool readFrameData(VideoFrameData& frameData);
    bool readSourceFrame();

    void updateState(VideoState state);

private:
    int mMaxBufferCount;
    ulong mBufferInterval;

    VideoState mState;
    bool mStopDecoding;
    QTimer mPlayTimer;

    VideoInfo mVideoInfo;
    VideoFramePacket mFramePacket;

    int mCurFrameIndex;
    int mGotoFrameIndex;

    mutable QReadWriteLock mFrameDataLock;
    QList<VideoFrameData> mFrameDatas;
};

} // namespace Video

#endif // VIDEODECODER_H
