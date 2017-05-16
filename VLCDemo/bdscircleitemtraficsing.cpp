/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    bdscircleitemtraficsing.cpp
 * @brief
 *******************************************************************************
 */
#include "bdscircleitemtraficsing.h"
#include <QPainterPath>

BDSCircleItemTraficsing::BDSCircleItemTraficsing(int size, int x, int y):GraphicsItemBase(size, x, y)
{
    m_color = QColor::fromHsv(rand() % 360, 255,255);
}

void BDSCircleItemTraficsing::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int dt = m_startTime.msecsTo(QTime::currentTime());

    qreal r0 = 0.5 * m_size * (1.0 - qExp(-0.001 * ((dt + 3800) % 4000)));
    qreal r1 = 0.5 * m_size * (1.0 - qExp(-0.001 * ((dt + 0) % 4000)));
    qreal r2 = 0.5 * m_size * (1.0 - qExp(-0.001 * ((dt + 1800) % 4000)));
    qreal r3 = 0.5 * m_size * (1.0 - qExp(-0.001 * ((dt + 2000) % 4000)));

    if(r0 > r1)
    {
        r0 = 0.0;
    }

    if(r2 > r3)
    {
        r2 = 0.0;
    }

    QPainterPath path;
    path.moveTo(r1, 0.0);
    path.arcTo(-r1, -r1, 2 * r1, 2 * r1, 0.0, 360.0);
    path.lineTo(r0, 0.0);
    path.arcTo(-r0, -r0, 2 * r0, 2 * r0, 0.0, -360.0);
    path.closeSubpath();
    path.moveTo(r3, 0.0);
    path.arcTo(-r3, -r3, 2 * r3, 2 * r3, 0.0, 360.0);
    path.lineTo(r0, 0.0);
    path.arcTo(-r2, -r2, 2 * r2, 2 * r2, 0.0, -360.0);
    path.closeSubpath();

    painter->beginNativePainting();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setBrush(QBrush(m_color));
    painter->setPen(Qt::NoPen);
    painter->drawPath(path);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(Qt::SolidLine);
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->endNativePainting();

    GraphicsItemBase::paint(painter,option, widget);
}

GraphicsItemBase *BDSCircleItemTraficsing::createNew(int size, int x, int y)
{
    return new BDSCircleItemTraficsing(size, x, y);
}
