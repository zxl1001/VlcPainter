/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    graphicsview.cpp
 * @brief
 *******************************************************************************
 */
#include "VideoView.h"
#include <QGLWidget>
#include <QGLFormat>
#include <QDebug>
#include <QTime>

namespace Video {

VideoView::VideoView(QWidget *parent)
    :QGraphicsView(parent),
      m_isEditMode(false)
{
    setFrameShape(QFrame::NoFrame);
    setRenderHint(QPainter::Antialiasing, true);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));

    m_scene = new VideoScene();
    setScene(m_scene);
    m_scene->setSceneRect(0,0,1024,768);
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    connect(&mVideoDecoder, &VideoDecoder::stateChanged,
            this, &VideoView::onVideoStateChanged);
    connect(&mVideoDecoder, &VideoDecoder::frameChanged,
            this, &VideoView::onVideoFrameChanged);
}

VideoView::~VideoView()
{
    if (m_scene != Q_NULLPTR)
    {
        delete m_scene;
        m_scene = Q_NULLPTR;
    }
}

void VideoView::setEditMode(bool isEditMode)
{
    if (isEditMode == m_isEditMode)
    {
        return;
    }

    m_isEditMode = isEditMode;
    if(m_isEditMode)
    {
        pause();
    }
}

bool VideoView::isEditMode() const
{
    return m_isEditMode;
}

void VideoView::setVideoBufferInterval(ulong bufferInterval)
{
    mVideoDecoder.setBufferInterval(bufferInterval);
}

void VideoView::setVideoMaxBufferCount(int maxCount)
{
    mVideoDecoder.setMaxBufferCount(maxCount);
}

ulong VideoView::getVideoBufferInterval() const
{
    return mVideoDecoder.getBufferInterval();
}

int VideoView::getVideoMaxBufferCount() const
{
    return mVideoDecoder.getMaxBufferCount();
}

bool VideoView::initializeVideo(const QString &filePath)
{
    m_scene->setVideoDecoder(&mVideoDecoder);
    if (!mVideoDecoder.initialize(filePath))
    {
        m_scene->setVideoDecoder(Q_NULLPTR);
        return false;
    }

    return true;
}

void VideoView::uninitializeVideo()
{
    m_scene->setVideoDecoder(Q_NULLPTR);
    mVideoDecoder.uninitialize();
}

bool VideoView::isVideoAvailable() const
{
    return mVideoDecoder.isVideoAvailable();
}

QString VideoView::getVideoFilePath() const
{
    return mVideoDecoder.getVideoFilePath();
}

qint64 VideoView::getVideoDuration() const
{
    return mVideoDecoder.getVideoDuration();
}

float VideoView::getFrameRate() const
{
    return mVideoDecoder.getFrameRate();
}

int VideoView::getFrameCount() const
{
    return mVideoDecoder.getFrameCount();
}

void VideoView::play()
{
    mVideoDecoder.play();
}

void VideoView::pause()
{
    mVideoDecoder.pause();
}

void VideoView::stop()
{
    mVideoDecoder.stop();
}

void VideoView::gotoFrame(int frameIndex)
{
    mVideoDecoder.gotoFrame(frameIndex);
}

void VideoView::gotoNextFrame()
{
    mVideoDecoder.gotoFrame(mVideoDecoder.getCurrentFrame() + 1);
}

void VideoView::gotoPrevFrame()
{
    mVideoDecoder.gotoFrame(mVideoDecoder.getCurrentFrame() - 1);
}

void VideoView::gotoTime(qint64 ms)
{
    mVideoDecoder.gotoTime(ms);
}

void VideoView::gotoPercent(float percent)
{
    mVideoDecoder.gotoPercent(percent);
}

bool VideoView::saveCurrentFrame(const QString &path) const
{
    return mVideoDecoder.saveCurrentFrame(path);
}

bool VideoView::isReady() const
{
    return mVideoDecoder.isReady();
}

bool VideoView::isPlaying() const
{
    return mVideoDecoder.isPlaying();
}

bool VideoView::isPaused() const
{
    return mVideoDecoder.isPaused();
}

int VideoView::getCurrentFrame() const
{
    return mVideoDecoder.getCurrentFrame();
}

int VideoView::getGotoFrame() const
{
    return mVideoDecoder.getGotoFrame();
}

qint64 VideoView::getCurrentTime() const
{
    return mVideoDecoder.getCurrentTime();
}

float VideoView::getCurrentPercent() const
{
    return mVideoDecoder.getCurrentPercent();
}

void VideoView::resizeEvent(QResizeEvent *event)
{
    if(m_scene != Q_NULLPTR)
    {
        m_scene->setSceneRectFromView(QRect(QPoint(0,0),event->size()));
    }
    event->accept( );
}

void VideoView::onVideoStateChanged(VideoState state)
{
    emit videoStateChanged(state);
}

void VideoView::onVideoFrameChanged(int index)
{
    m_scene->updateSceneByFrame(index);

    int frameCount = mVideoDecoder.getFrameCount();
    if (frameCount > 0)
    {
        emit videoFrameChanged(index);
    }
}

VideoScene *VideoView::getScene() const
{
    return m_scene;
}

//name space end
}
