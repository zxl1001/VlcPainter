/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    VideoDecoder.cpp
 *******************************************************************************
 */

#include "VideoDecoder.h"

namespace Video
{

VideoInfo::VideoInfo()
    : avFormatContext(Q_NULLPTR)
    , avStreamIndex(-1)
    , frameRate(0)
    , deltaTimeStamp(0)
    , avStream(Q_NULLPTR)
    , avCodecContext(Q_NULLPTR)
    , avCodec(Q_NULLPTR)
    , swsContext(Q_NULLPTR)
{
}

VideoFrameData::VideoFrameData()
    : index(-1)
    , timestamp(0)
{
}

VideoFramePacket::VideoFramePacket()
    : avSrcFrame(Q_NULLPTR)
    , avDstFrame(Q_NULLPTR)
{
}

VideoDecoder::VideoDecoder()
    : mMaxBufferCount(50)
    , mBufferInterval(1)
    , mState(VideoState::Invalid)
    , mStopDecoding(false)
    , mCurFrameIndex(-1)
    , mGotoFrameIndex(-1)
{
    avcodec_register_all();
    av_register_all();

    qRegisterMetaType<VideoState>("VideoState");

    connect(&mPlayTimer, &QTimer::timeout, this, &VideoDecoder::onPlayTimer);
}

VideoDecoder::~VideoDecoder()
{
    uninitialize();
}

void VideoDecoder::setBufferInterval(ulong bufferInterval)
{
    mBufferInterval = bufferInterval;
}

void VideoDecoder::setMaxBufferCount(int maxCount)
{
    mMaxBufferCount = (maxCount > 50 ? maxCount : 50);
}

ulong VideoDecoder::getBufferInterval() const
{
    return mBufferInterval;
}

int VideoDecoder::getMaxBufferCount() const
{
    return mMaxBufferCount;
}

bool VideoDecoder::initialize(const QString& filePath)
{
    if (isPlaying() || isPaused())
    {
        return false;
    }

    if (filePath.isEmpty())
    {
        return false;
    }

    uninitialize();

    if (avformat_open_input(&mVideoInfo.avFormatContext,
                            filePath.toStdString().c_str(), Q_NULLPTR, Q_NULLPTR)
        != 0)
    {
        return false;
    }

    if (avformat_find_stream_info(mVideoInfo.avFormatContext, Q_NULLPTR) < 0)
    {
        return false;
    }

    for (int i = 0; mVideoInfo.avFormatContext->nb_streams; ++i)
    {
        if (mVideoInfo.avFormatContext->streams[i]->codec->codec_type
            == AVMEDIA_TYPE_VIDEO)
        {
            mVideoInfo.avStreamIndex = i;
            mVideoInfo.avStream =
                    mVideoInfo.avFormatContext->streams[mVideoInfo.avStreamIndex];
            break;
        }
    }
    if (mVideoInfo.avStreamIndex < 0)
    {
        return false;
    }

    mVideoInfo.frameRate = static_cast<float>(mVideoInfo.avStream->r_frame_rate.num)
                           / mVideoInfo.avStream->r_frame_rate.den;
    mVideoInfo.deltaTimeStamp = (mVideoInfo.avStream->time_base.den
                                 / mVideoInfo.avStream->time_base.num)
                                / mVideoInfo.frameRate;

    mVideoInfo.avCodecContext = mVideoInfo.avStream->codec;
    mVideoInfo.avCodec = avcodec_find_decoder(mVideoInfo.avCodecContext->codec_id);
    if (mVideoInfo.avCodec == Q_NULLPTR)
    {
        return false;
    }

    if (avcodec_open2(mVideoInfo.avCodecContext, mVideoInfo.avCodec, NULL) < 0)
    {
        return false;
    }

    av_new_packet(&mFramePacket.avPacket,
                  mVideoInfo.avCodecContext->width
                          * mVideoInfo.avCodecContext->height);
    mVideoInfo.swsContext = sws_getContext(
            mVideoInfo.avCodecContext->width, mVideoInfo.avCodecContext->height,
            mVideoInfo.avCodecContext->pix_fmt,
            mVideoInfo.avCodecContext->width, mVideoInfo.avCodecContext->height,
            AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);

#if LIBAVCODEC_VERSION_INT >= AV_VERSION_INT(55, 28, 1)
    mFramePacket.avSrcFrame = av_frame_alloc();
    mFramePacket.avDstFrame = av_frame_alloc();
#else
    mFramePacket.avSrcFrame = avcodec_alloc_frame();
    mFramePacket.avDstFrame = avcodec_alloc_frame();
#endif

    uint8_t* out_buffer;
    out_buffer =
            new uint8_t[avpicture_get_size(AV_PIX_FMT_RGB32,
                                           mVideoInfo.avCodecContext->width,
                                           mVideoInfo.avCodecContext->height)];
    avpicture_fill((AVPicture*)mFramePacket.avDstFrame, out_buffer,
                   AV_PIX_FMT_RGB32, mVideoInfo.avCodecContext->width,
                   mVideoInfo.avCodecContext->height);
    avpicture_fill((AVPicture*)mFramePacket.avSrcFrame, out_buffer,
                   AV_PIX_FMT_RGB32, mVideoInfo.avCodecContext->width,
                   mVideoInfo.avCodecContext->height);

    mVideoInfo.filePath = filePath;
    mCurFrameIndex = 0;
    mStopDecoding = false;

    gotoFrameInternal(0);
    updateState(VideoState::Paused);
    if (!isRunning())
    {
        start();
    }

    return true;
}

void VideoDecoder::uninitialize()
{
    mState = VideoState::Invalid;

    mStopDecoding = true;
    this->wait(1000);

    mCurFrameIndex = -1;
    mVideoInfo.avStreamIndex = -1;
    mVideoInfo.frameRate = 0;
    mVideoInfo.deltaTimeStamp = 0;
    mVideoInfo.avStream = Q_NULLPTR;
    mVideoInfo.avCodec = Q_NULLPTR;
    mVideoInfo.filePath.clear();

    if (mVideoInfo.swsContext != Q_NULLPTR)
    {
        sws_freeContext(mVideoInfo.swsContext);
        mVideoInfo.swsContext = Q_NULLPTR;
    }

#if LIBAVCODEC_VERSION_INT >= AV_VERSION_INT(55, 28, 1)
    if (mFramePacket.avSrcFrame != Q_NULLPTR)
    {
        av_frame_free(&mFramePacket.avSrcFrame);
        mFramePacket.avSrcFrame = Q_NULLPTR;
    }
    if (mFramePacket.avDstFrame != Q_NULLPTR)
    {
        av_frame_free(&mFramePacket.avDstFrame);
        mFramePacket.avDstFrame = Q_NULLPTR;
    }
#else
    if (mFramePacket.avSrcFrame != Q_NULLPTR)
    {
        avcodec_frame_free(&mFramePacket.avSrcFrame);
        mFramePacket.avSrcFrame = Q_NULLPTR;
    }
    if (mFramePacket.avDstFrame != Q_NULLPTR)
    {
        avcodec_frame_free(&mFramePacket.avDstFrame);
        mFramePacket.avDstFrame = Q_NULLPTR;
    }
#endif

    if (mVideoInfo.avCodecContext != Q_NULLPTR)
    {
        avcodec_close(mVideoInfo.avCodecContext);
        avcodec_free_context(&mVideoInfo.avCodecContext);
        mVideoInfo.avCodecContext = Q_NULLPTR;
    }

    if (mVideoInfo.avFormatContext != Q_NULLPTR)
    {
        mVideoInfo.avFormatContext = Q_NULLPTR;
    }

    clearAllFrames();
}

bool VideoDecoder::isVideoAvailable() const
{
    return (mVideoInfo.avStream != Q_NULLPTR);
}

const QString &VideoDecoder::getVideoFilePath() const
{
    return mVideoInfo.filePath;
}

qint64 VideoDecoder::getVideoDuration() const
{
    if (mVideoInfo.avFormatContext == Q_NULLPTR)
    {
        return 0;
    }

    return mVideoInfo.avFormatContext->duration / 1000;
}

float VideoDecoder::getFrameRate() const
{
    return mVideoInfo.frameRate;
}

int VideoDecoder::getFrameCount() const
{
    if (mVideoInfo.avStream == Q_NULLPTR)
    {
        return 0;
    }

    return mVideoInfo.avStream->nb_frames;
}

bool VideoDecoder::play()
{
    if ((mState != VideoState::Ready) && (mState != VideoState::Paused))
    {
        return false;
    }

    if (mState == VideoState::Ready)
    {
        gotoFrameInternal(0);
        mGotoFrameIndex = -1;
    }

    mStopDecoding = false;
    updateState(VideoState::Playing);
    if (!isRunning())
    {
        start();
    }
    mPlayTimer.start(static_cast<int>(mVideoInfo.frameRate));

    return true;
}

bool VideoDecoder::pause()
{
    if (mState != VideoState::Playing)
    {
        return false;
    }

    mPlayTimer.stop();
    updateState(VideoState::Paused);

    return true;
}

bool VideoDecoder::stop()
{
    if ((mState != VideoState::Playing) && (mState != VideoState::Paused))
    {
        return false;
    }

    mCurFrameIndex = 0;
    mStopDecoding = true;
    mPlayTimer.stop();
    wait(1000);
    updateState(VideoState::Ready);
    gotoFrameInternal(0);

    return true;
}

bool VideoDecoder::gotoFrame(int frameIndex)
{
    if (mVideoInfo.deltaTimeStamp <= 0)
    {
        return false;
    }

    if (frameIndex < 0)
    {
        mGotoFrameIndex = 0;
    }
    else if (frameIndex >= getFrameCount())
    {
        mGotoFrameIndex = getFrameCount() - 1;
    }
    else
    {
        mGotoFrameIndex = frameIndex;
    }

    return true;
}

bool VideoDecoder::gotoTime(qint64 ms)
{
    qint64 duration = getVideoDuration();
    if (duration <= 0)
    {
        return false;
    }

    return gotoFrame(static_cast<int>((static_cast<float>(ms) / duration) * getFrameCount()));
}

bool VideoDecoder::gotoPercent(float percent)
{
    return gotoFrame(static_cast<int>(percent * getFrameCount()));
}

VideoState VideoDecoder::getState() const
{
    return mState;
}

bool VideoDecoder::isReady() const
{
    return (mState == VideoState::Ready);
}

bool VideoDecoder::isPlaying() const
{
    return (mState == VideoState::Playing);
}

bool VideoDecoder::isPaused() const
{
    return (mState == VideoState::Paused);
}

int VideoDecoder::getCurrentFrame() const
{
    return mCurFrameIndex;
}

int VideoDecoder::getGotoFrame() const
{
    return mGotoFrameIndex;
}

qint64 VideoDecoder::getCurrentTime() const
{
    int frameCount = getFrameCount();
    if (frameCount <= 0)
    {
        return 0;
    }

    return static_cast<qint64>((static_cast<float>(mCurFrameIndex) / frameCount) * getVideoDuration());
}

float VideoDecoder::getCurrentPercent() const
{
    int frameCount = getFrameCount();
    if (frameCount <= 0)
    {
        return 0.0;
    }

    return static_cast<float>(mCurFrameIndex) / frameCount;
}

bool VideoDecoder::hasFrameData(int index) const
{
    QReadLocker locker(&mFrameDataLock);
    foreach(const VideoFrameData& iter, mFrameDatas)
    {
        if (iter.index == index)
        {
            return true;
        }
    }

    return false;
}

bool VideoDecoder::getFrameData(int index, VideoFrameData &frameData, bool matchIndex) const
{
    QReadLocker locker(&mFrameDataLock);
    if (mFrameDatas.isEmpty())
    {
        return false;
    }

    const VideoFrameData* data = Q_NULLPTR;
    if (index < mFrameDatas.first().index)
    {
        if (matchIndex)
        {
            return false;
        }
        data = &mFrameDatas.first();
    }
    else if (index > mFrameDatas.last().index)
    {
        if (matchIndex)
        {
            return false;
        }
        data = &mFrameDatas.last();
    }
    else
    {
        foreach(const VideoFrameData& iter, mFrameDatas)
        {
            if (iter.index == index)
            {
                data = &iter;
                break;
            }
        }
    }

    if (data == Q_NULLPTR)
    {
        return false;
    }

    frameData.index = data->index;
    frameData.timestamp = data->timestamp;
    frameData.image = data->image.copy();

    return true;
}

bool VideoDecoder::saveCurrentFrame(const QString &path) const
{
    if (path.isEmpty())
    {
        return false;
    }

    VideoFrameData frameData;
    if (!getFrameData(mCurFrameIndex, frameData))
    {
        return false;
    }

    return frameData.image.save(path);
}

void VideoDecoder::run()
{
    while (!mStopDecoding)
    {
        msleep(mBufferInterval);

        if (mGotoFrameIndex >= 0)
        {
            gotoFrameInternal(mGotoFrameIndex);
            if (mGotoFrameIndex == mCurFrameIndex)
            {
                mGotoFrameIndex = -1;
            }
        }

        clearFramesOutOfBuffer();

        int maxFrameIndex = mCurFrameIndex + mMaxBufferCount;
        if (maxFrameIndex >= getFrameCount())
        {
            maxFrameIndex = getFrameCount() - 1;
        }
        if (hasFrameData(maxFrameIndex))
        {
            continue;
        }

        VideoFrameData frameData;
        if (!readFrameData(frameData))
        {
            continue;
        }

        if (!addFrameData(frameData, true))
        {
            seekToTimeStampOnce(static_cast<int64_t>((mCurFrameIndex + 1) * mVideoInfo.deltaTimeStamp));
        }
    }

    clearAllFrames();
}

void VideoDecoder::onPlayTimer()
{
    if (mState == VideoState::Playing)
    {
        ++mCurFrameIndex;
        if (mCurFrameIndex >= getFrameCount())
        {
            mState = VideoState::Paused;
            stop();
        }
    }

    emit frameChanged(mCurFrameIndex);
}

bool VideoDecoder::addFrameData(const VideoFrameData &frameData, bool checkSequence)
{
    QWriteLocker locker(&mFrameDataLock);
    QList<VideoFrameData>::Iterator iter = mFrameDatas.begin();
    for (; iter != mFrameDatas.end(); ++iter)
    {
        if (frameData.index < iter->index)
        {
            mFrameDatas.insert(iter, frameData);
            return true;
        }
        else if (frameData.index == iter->index)
        {
            return true;
        }
    }

    if (checkSequence)
    {
        if (!mFrameDatas.isEmpty())
        {
            if (frameData.index > mFrameDatas.last().index + 1)
            {
                return false;
            }
        }
    }

    mFrameDatas.append(frameData);
    return true;
}

void VideoDecoder::clearFramesOutOfBuffer()
{
    QWriteLocker locker(&mFrameDataLock);
    QList<VideoFrameData>::Iterator iter = mFrameDatas.begin();
    int minIndex = mCurFrameIndex - mMaxBufferCount;
    int maxIndex = mCurFrameIndex + mMaxBufferCount;
    if (minIndex < 0)
    {
        minIndex = 0;
    }
    if (maxIndex >= getFrameCount())
    {
        maxIndex = getFrameCount() - 1;
    }
    while (iter != mFrameDatas.end())
    {
        if ((iter->index < minIndex) || (iter->index > maxIndex))
        {
            iter = mFrameDatas.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void VideoDecoder::clearAllFrames()
{
    QWriteLocker locker(&mFrameDataLock);
    mFrameDatas.clear();
}

bool VideoDecoder::gotoFrameInternal(int frameIndex)
{
    if (!hasFrameData(frameIndex))
    {
        gotoTimeStamp(static_cast<int64_t>((frameIndex + 1) * mVideoInfo.deltaTimeStamp));
    }

    mCurFrameIndex = frameIndex;
    if (mState != VideoState::Playing)
    {
        emit frameChanged(mCurFrameIndex);
    }

    return true;
}

bool VideoDecoder::gotoTimeStamp(int64_t pts)
{
    VideoFrameData frameData;
    if (!gotoTimeStampBackward(pts, frameData))
    {
        return false;
    }

    do
    {
        if (frameData.index < 0)
        {
            continue;
        }

        addFrameData(frameData, false);
        if (frameData.timestamp >= pts)
        {
            return true;
        }

    } while (readFrameData(frameData));

    return false;
}

bool VideoDecoder::gotoTimeStampBackward(int64_t pts, VideoFrameData& frameData)
{
    if (pts < 0)
    {
        pts = 0;
    }

    int64_t ptsSeek = pts;
    while (ptsSeek >= static_cast<int64_t>(-mVideoInfo.deltaTimeStamp))
    {
        if (!seekToTimeStampOnce(ptsSeek))
        {
            return false;
        }

        if (!readFrameData(frameData))
        {
            return false;
        }

        if (frameData.timestamp >= static_cast<int64_t>(pts + mVideoInfo.deltaTimeStamp))
        {
            ptsSeek -= static_cast<int64_t>(mVideoInfo.deltaTimeStamp);
            continue;
        }

        break;
    }

    return true;
}

bool VideoDecoder::seekToTimeStampOnce(int64_t pts)
{
    int result = av_seek_frame(mVideoInfo.avFormatContext, mVideoInfo.avStreamIndex,
                           pts, AVSEEK_FLAG_BACKWARD);
    return (result >= 0);
}

bool VideoDecoder::readFrameData(VideoFrameData& frameData)
{
    if (!readSourceFrame())
    {
        return false;
    }

    sws_scale(mVideoInfo.swsContext,
              (const uint8_t* const*)mFramePacket.avSrcFrame->data,
              mFramePacket.avSrcFrame->linesize, 0,
              mVideoInfo.avCodecContext->height, mFramePacket.avDstFrame->data,
              mFramePacket.avDstFrame->linesize);

    frameData.index = static_cast<int>(mFramePacket.avSrcFrame->pts / mVideoInfo.deltaTimeStamp - 1);
    frameData.timestamp = mFramePacket.avSrcFrame->pts;
    frameData.image =
            QImage((uchar*)mFramePacket.avDstFrame->data[0],
                   mVideoInfo.avCodecContext->width,
                   mVideoInfo.avCodecContext->height, QImage::Format_RGB32)
                    .copy();

    return true;
}

bool VideoDecoder::readSourceFrame()
{
    if (av_read_frame(mVideoInfo.avFormatContext, &mFramePacket.avPacket) < 0)
    {
        return false;
    }

    if (mFramePacket.avPacket.stream_index != mVideoInfo.avStreamIndex)
    {
        return false;
    }

    int gotPicture = 0;
    int packetLen = avcodec_decode_video2(mVideoInfo.avCodecContext,
                                          mFramePacket.avSrcFrame, &gotPicture,
                                          &mFramePacket.avPacket);
    if (packetLen <= 0)
    {
        return false;
    }

    return true;
}

void VideoDecoder::updateState(VideoState state)
{
    mState = state;
    emit stateChanged(mState);
}

} // namespace Video
