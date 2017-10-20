/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    ComboBoxHeaderView.cpp
 * @brief
 *******************************************************************************
 */
#include "ComboBoxHeaderView.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QMenu>

ComboBoxHeaderView::ComboBoxHeaderView(Qt::Orientation orientation, QWidget* parent)
    : QHeaderView(orientation, parent)
{
    qDebug() << "Qt::Orientation" << orientation;
}

ComboBoxHeaderView::~ComboBoxHeaderView()
{
    qDebug() << "~ComboBoxHeaderView()";
}

void ComboBoxHeaderView::paintSection(QPainter* painter, const QRect& rect,
                                      int logicalIndex) const
{
    qDebug()<<rect.x()<<rect.y()<<logicalIndex;
    painter->save();
    QHeaderView::paintSection(painter, rect, logicalIndex);
    painter->restore();
    if (logicalIndex == 2)
    {
          mPosX = rect.x();
          mPosY = rect.y()+rect.height();
//        //进度条的风格选项
//        QStyleOptionProgressBarV2 *progressBarOption = new QStyleOptionProgressBarV2();
//        progressBarOption->rect = rect;
//        progressBarOption->minimum = 1;
//        progressBarOption->maximum = 100;
//        progressBarOption->progress = 23;
//        progressBarOption->text = QString::number(23) + "%";
//        progressBarOption->textVisible = true;
//        this->style()->drawControl(QStyle::CE_ProgressBar, progressBarOption, painter);
    }
}

void ComboBoxHeaderView::mousePressEvent(QMouseEvent* e)
{
    mIx = e->pos().x();
    mIy = e->pos().y();

//    int iSecondColumn = sectionPosition(2);
    if(visualIndexAt(mIx) == 2)
    {
        QMenu menu;
        menu.addAction("AAA");
        menu.addAction("BBB");
        menu.addAction("CCC");
        menu.addAction("DDD");
        menu.addAction("EEE");
        menu.exec(this->mapToGlobal(QPoint(mPosX, mPosY)));
    }
}

void ComboBoxHeaderView::paintEvent(QPaintEvent* e)
{
    if( mIsMousePress )
    {
        mIsMousePress = false;
        if ( mIsOn ){
            mStyle = QStyle::State_On;
        }
        else
        {
            mStyle = QStyle::State_Off;
        }
    }
    QHeaderView::paintEvent(e);
}
