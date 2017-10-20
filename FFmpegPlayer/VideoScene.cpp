/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    graphicsscene.cpp
 * @brief
 *******************************************************************************
 */
#include "VideoScene.h"

#include <QDebug>
#include <QTime>

static const int g_tagSizeStep = 5;

namespace Video
{
VideoScene::VideoScene(QObject* parent)
    : QGraphicsScene(parent)
    , m_isMove(false)
    , m_oldPoint(0, 0)
    , m_curFrameIdx(0)
    , m_tm(0)
    , m_videoDecoder(Q_NULLPTR)
{
    setBackgroundBrush(Qt::black);
    setBspTreeDepth(1);
    connect(this, &VideoScene::sceneRectChanged, this,
            &VideoScene::slotSceneRectChanged);
}

VideoScene::~VideoScene()
{
}

void VideoScene::setSceneRectFromView(const QRect& rect)
{
    setSceneRect(rect);
    update(rect);
}

void VideoScene::setVideoDecoder(const VideoDecoder* videoDecoder)
{
    m_videoDecoder = videoDecoder;
    update();
}

void VideoScene::updateSceneByFrame(int frameIdx)
{
    if (m_videoDecoder == Q_NULLPTR)
    {
        return;
    }

    int frameCount = m_videoDecoder->getFrameCount();
    if (frameCount < 1)
    {
        return;
    }

    VideoFrameData frameData;
    if (m_videoDecoder->getFrameData(frameIdx, frameData))
    {
        m_frameImage = frameData.image;
    }

    m_curFrameIdx = frameIdx;
    m_tm = static_cast<qint64>(m_videoDecoder->getVideoDuration() * frameIdx
                               / (float)m_videoDecoder->getFrameCount());

    update();
}

void VideoScene::drawBackground(QPainter* painter, const QRectF& rect)
{
    if ((m_videoDecoder == Q_NULLPTR) || m_frameImage.isNull())
    {
        QPixmap img(":/framework/pics/bkgnd.png");
        QSize imgSize = img.size();
        QPoint pt((rect.width() - imgSize.width()) / 2,
                  (rect.height() - imgSize.height()) / 2);
        if (pt.x() < 0)
        {
            pt.setX(0);
        }
        if (pt.y() < 0)
        {
            pt.setY(0);
        }

        painter->beginNativePainting();
        painter->fillRect(rect, QBrush(QColor(0xFFCCCCCC)));
        painter->drawPixmap(pt, img, rect);
        painter->endNativePainting();
        return;
    }
    painter->beginNativePainting();
    painter->drawImage(QPointF(rect.x(), rect.y()),
                       m_frameImage.scaled(rect.width(), rect.height()));

    // draw the attribute
    if (m_videoDecoder != Q_NULLPTR)
    {
        QRect attrRect(20, 20, 150, 50);
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setBrush(QBrush(QColor(48, 48, 48, 96)));
        painter->setPen(QPen(QBrush(Qt::white), 0.5, Qt::SolidLine));
        painter->drawRoundedRect(attrRect.marginsAdded(QMargins(10, 10, 0, 0)),
                                 5, 5);
        painter->restore();

        QFont font;
        painter->save();
        font.setPixelSize(11);
        painter->setFont(font);
        painter->setPen(Qt::white);
        painter->drawText(
                attrRect,
                QString().sprintf("Frame: %d / %d\nTime: %s\nFPS: %.1f",
                                  m_curFrameIdx + 1,
                                  m_videoDecoder->getFrameCount(),
                                  QTime::fromMSecsSinceStartOfDay(m_tm)
                                          .toString("mm:ss.zzz")
                                          .toStdString()
                                          .c_str(),
                                  m_videoDecoder->getFrameRate()));
        painter->restore();
    }

    painter->endNativePainting();
}

void VideoScene::slotSceneRectChanged(const QRectF& rect)
{
}

void VideoScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
}

void VideoScene::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
        case Qt::Key_Plus:
        case Qt::Key_Equal:
        {
            break;
        }
        case Qt::Key_Minus:
        {
            break;
        }
    }
}
// name space end
}
