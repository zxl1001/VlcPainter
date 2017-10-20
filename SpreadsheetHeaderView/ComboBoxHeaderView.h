/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    ComboBoxHeaderView.h
 * @brief
 *******************************************************************************
 */
#ifndef COMBOBOXHEADERVIEW_H
#define COMBOBOXHEADERVIEW_H

#include <QHeaderView>
#include <QRect>
#include <QPoint>

class ComboBoxHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    ComboBoxHeaderView(Qt::Orientation orientation, QWidget* parent = Q_NULLPTR);
    virtual ~ComboBoxHeaderView();

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);

private:
    bool mIsOn;
    int mIx;
    int mIy;

    mutable int mPosX;
    mutable int mPosY;
    bool mIsMousePress; // weather press
    int  mStyle;        // style
};

#endif // COMBOBOXHEADERVIEW_H
