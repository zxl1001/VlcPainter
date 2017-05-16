/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    bdslimit30traficsign.cpp
 * @brief
 *******************************************************************************
 */
#include "bdslimit30traficsign.h"

BDSLimit30TraficSign::BDSLimit30TraficSign(int size, int x, int y)
    :GraphicsItemBase(size, x, y)
{

}

void BDSLimit30TraficSign::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::green,3));
    painter->drawRect(-m_size / 2, -m_size / 2, m_size, m_size);
    GraphicsItemBase::paint(painter, option, widget);
}

GraphicsItemBase *BDSLimit30TraficSign::createNew(int size, int x, int y)
{
    return new BDSLimit30TraficSign(size, x, y);
}
