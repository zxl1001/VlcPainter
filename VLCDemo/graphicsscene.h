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
#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H
#include <vlc/vlc.h>

#include "bdslimit30traficsign.h"
#include "bdsrounditemtraficsign.h"
#include "bdscircleitemtraficsing.h"
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>
#include <QImage>
namespace GraphicsVideo {
class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject * parent = 0);
    virtual ~GraphicsScene();

    void updatePicture(const QImage &image);


    uint64_t tm() const;
    void setTm(const uint64_t &tm);

    float persent() const;
    void setPersent(float persent);

    float fps() const;
    void setFps(float fps);

    const QPixmap &FramePix() const;

    qint64 duration() const;
    void setDuration(const qint64 &duration);


protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    QPixmap      m_FramePix;
    uint64_t     m_tm;
    float        m_persent;
    float        m_fps;
    qint64       m_duration;
    QRectF       m_rect;
    bool         m_isMove;
    QPointF      m_oldPoint;
    BDSLimit30TraficSign *m_rectItem1;
    BDSRoundItemTraficsign *m_rectItem2;
    BDSCircleItemTraficsing *m_rectItem3;
    QGraphicsProxyWidget *m_widgetItem;
};
}
#endif // GRAPHICSSCENE_H
