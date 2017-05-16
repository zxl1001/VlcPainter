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
#include "graphicsscene.h"

#include <QTime>
#include <QDebug>
namespace GraphicsVideo {
GraphicsScene::GraphicsScene(QObject *parent)
    :QGraphicsScene(parent),
      m_isMove(false),
      m_oldPoint(0,0),
      m_rectItem1(new BDSLimit30TraficSign(100,300,100)),
      m_rectItem2(new BDSRoundItemTraficsign(100,200,100))
{
    //set scene parame


    m_persent = 0;
    m_fps     = 0;
    m_tm      = 0;
//    m_rectItem1 = new QGraphicsEllipseItem(100,100,100,100);
//    m_rectItem1->setPen(QPen(Qt::yellow, 5));
//    m_rectItem1->setFlag(QGraphicsItem::ItemIsMovable, true);
//    m_rectItem1->setFlag(QGraphicsItem::ItemIsSelectable, true);
//    m_rectItem1->setFlag(QGraphicsItem::ItemIsFocusable, true);
//    addItem(m_rectItem1);

    addItem(m_rectItem1);
    addItem(m_rectItem2);

}

GraphicsScene::~GraphicsScene()
{

}

void GraphicsScene::updatePicture(const QImage &image)
{
    m_FramePix = QPixmap::fromImage(image);
    update();
}

uint64_t GraphicsScene::tm() const
{
    return m_tm;
}

void GraphicsScene::setTm(const uint64_t &tm)
{
    m_tm = tm;
}

float GraphicsScene::persent() const
{
    return m_persent;
}

void GraphicsScene::setPersent(float persent)
{
    m_persent = persent;
}

float GraphicsScene::fps() const
{
    return m_fps;
}

void GraphicsScene::setFps(float fps)
{
    m_fps = fps;
}

const QPixmap &GraphicsScene::FramePix() const
{
    return m_FramePix;
}

void GraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
//    qDebug()<<"rect:"<<rect<<sceneRect();
    painter->beginNativePainting( );
//    painter->drawPixmap(0,0,m_FramePix);
    painter->drawPixmap(QPointF(rect.x(), rect.y()), m_FramePix.scaled(rect.width(), rect.height()));
    //draw the attribute
    QRect attrRect(20,20,300,100);
    painter->save();
    painter->setBrush(QBrush(QColor(0,0,0,80)));
    painter->setPen(Qt::white);
    painter->drawRoundedRect(attrRect.marginsAdded(QMargins(10,10,0,0)),5,5);
    painter->restore();

    QFont font;
    QTime time(0,0,0);
    painter->save();
    font.setPixelSize(24);
    painter->setFont(font);
    painter->setPen(Qt::white);
    painter->drawText(attrRect,
                     QString("Persent: %1\nFps: %2\nPosition: %3")
                     .arg(m_persent).arg(m_fps)
                     .arg(time.addMSecs(m_tm).toString("hh:mm:ss.zzz"))
                     );
    painter->restore();
    painter->endNativePainting();
}
//name space end
}
