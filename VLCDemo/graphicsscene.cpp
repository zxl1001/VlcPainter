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
#include "TagCategoryDialog.h"

#include <QTime>
#include <QDebug>
namespace GraphicsVideo {
GraphicsScene::GraphicsScene(QObject *parent)
    :QGraphicsScene(parent),
      m_isMove(false),
      m_oldPoint(0,0),
      m_rectItem1(new BDSLimit30TraficSign(100,300,100)),
      m_rectItem2(new BDSRoundItemTraficsign(100,200,100)),
      m_rectItem3(new BDSCircleItemTraficsing(100,400,100)),
      m_lastIdx(0)
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
    m_widgetItem = addWidget(new Video::TagCategoryDialog, Qt::SubWindow | Qt::WindowStaysOnTopHint);;
    m_widgetItem->setFlag(QGraphicsItem::ItemIsMovable, true);
    m_widgetItem->moveBy(100,100);
    addItem(m_rectItem1);
    addItem(m_rectItem2);
    addItem(m_rectItem3);
    for(int i =0; i<10000; ++i)
    {
        m_itemPos.append(QPointF(i, i+1));
    }
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

    int idx = (m_duration / 1000 * m_fps * m_persent);
    qDebug()<<"curIdx:"<<idx<<m_itemPos[idx]<<"widget:"<<m_rectItem1->pos();
    if(m_rectItem1->isPressed())
    {
        for(int i = m_lastIdx; i<= idx; ++i)
        {
            m_itemPos[i] = m_rectItem1->pos();
        }
        m_lastIdx = idx;
    }
    else
    {
        m_rectItem1->setPos(0,0);
        m_rectItem1->moveBy(m_itemPos[idx].x(), m_itemPos[idx].y());
    }
    m_widgetItem->setPos(rect.width()-m_widgetItem->size().width(), (rect.height()-m_widgetItem->size().height()) / 2);

    painter->beginNativePainting( );
//    painter->drawPixmap(0,0,m_FramePix);
    painter->drawPixmap(QPointF(rect.x(), rect.y()), m_FramePix.scaled(rect.width(), rect.height()));
    //draw the attribute
    QRect attrRect(20, 20, 300, 135);
    painter->save();
    painter->setBrush(QBrush(QColor(0,0,0,80)));
    painter->setPen(Qt::white);
    painter->drawRoundedRect(attrRect.marginsAdded(QMargins(10,10,0,0)), 5, 5);
    painter->restore();

    QFont font;
    QTime time(0,0,0);
    painter->save();
    font.setPixelSize(24);
    painter->setFont(font);
    painter->setPen(Qt::white);
    painter->drawText(attrRect,
                     QString("Persent: %1\nFps: %2\nPosition: %3\nDuration:%4")
                     .arg(m_persent).arg(m_fps)
                     .arg(time.addMSecs(m_tm).toString("hh:mm:ss.zzz"))
                     .arg(m_duration)
                     );
    painter->restore();
    painter->endNativePainting();
}

qint64 GraphicsScene::duration() const
{
    return m_duration;
}

void GraphicsScene::setDuration(const qint64 &duration)
{
    m_duration = duration;
}
//name space end
}
