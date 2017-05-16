/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    bdsrounditemtraficsign.h
 * @brief
 *******************************************************************************
 */
#ifndef BDSROUNDITEMTRAFICSIGN_H
#define BDSROUNDITEMTRAFICSIGN_H

#include "graphicsitembase.h"

class BDSRoundItemTraficsign : public GraphicsItemBase
{
public:
    BDSRoundItemTraficsign(int size, int x, int y);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
protected:
    virtual GraphicsItemBase *createNew(int size, int x, int y) Q_DECL_OVERRIDE;
};

#endif // BDSROUNDITEMTRAFICSIGN_H
