/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    graphicsscene.h
 * @brief
 *******************************************************************************
 */
#ifndef VIDEOSCENE_H
#define VIDEOSCENE_H

#include "VideoDecoder.h"

#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>
#include <QImage>
#include <QHash>

namespace Video {
class VideoScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit VideoScene(QObject * parent = 0);
    virtual ~VideoScene();

    void setSceneRectFromView(const QRect &rect);

    void setVideoDecoder(const VideoDecoder* videoDecoder);
    void updateSceneByFrame(int frameIdx);

Q_SIGNALS:


protected slots:
    void slotSceneRectChanged(const QRectF &rect);

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;

private:
    QImage       m_frameImage;
    int          m_curFrameIdx;
    qint64       m_tm;

    QRectF       m_rect;
    bool         m_isMove;
    QPointF      m_oldPoint;

    QGraphicsProxyWidget *m_categoryProxyWidget;
    const VideoDecoder* m_videoDecoder;
};
}
#endif // VIDEOSCENE_H
