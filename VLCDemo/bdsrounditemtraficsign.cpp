/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    bdsrounditemtraficsign.cpp
 * @brief
 *******************************************************************************
 */
#include "bdsrounditemtraficsign.h"

BDSRoundItemTraficsign::BDSRoundItemTraficsign(int size, int x, int y)
    :GraphicsItemBase(size, x, y)
{

}

void BDSRoundItemTraficsign::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->beginNativePainting();
    painter->setPen(QPen(Qt::green,2));
    painter->drawEllipse(-m_size / 2, -m_size / 2, m_size, m_size);
    painter->endNativePainting();
    GraphicsItemBase::paint(painter, option, widget);
}

GraphicsItemBase *BDSRoundItemTraficsign::createNew(int size, int x, int y)
{
    return new BDSRoundItemTraficsign(size, x, y);
}
