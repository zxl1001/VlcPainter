/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    bdslimit30traficsign.h
 * @brief
 *******************************************************************************
 */
#ifndef BDSLIMIT30TRAFICSIGN_H
#define BDSLIMIT30TRAFICSIGN_H

#include "graphicsitembase.h"

class BDSLimit30TraficSign : public GraphicsItemBase
{
public:
    BDSLimit30TraficSign(int size, int x, int y);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
protected:
    virtual GraphicsItemBase *createNew(int size, int x, int y) Q_DECL_OVERRIDE;
};

#endif // BDSLIMIT30TRAFICSIGN_H
