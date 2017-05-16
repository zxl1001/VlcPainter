/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    bdscircleitemtraficsing.h
 * @brief
 *******************************************************************************
 */
#ifndef BDSCIRCLEITEMTRAFICSING_H
#define BDSCIRCLEITEMTRAFICSING_H

#include "graphicsitembase.h"

class BDSCircleItemTraficsing : public GraphicsItemBase
{
public:
    BDSCircleItemTraficsing(int size, int x, int y);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

protected:
    virtual GraphicsItemBase *createNew(int size, int x, int y) Q_DECL_OVERRIDE;
    QColor m_color;
};

#endif // BDSCIRCLEITEMTRAFICSING_H
