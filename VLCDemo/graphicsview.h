/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    graphicsview.h
 * @brief
 *******************************************************************************
 */
#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H
#include "graphicsscene.h"
#include <QGraphicsView>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QMutex>

namespace GraphicsVideo {

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent=0);
    virtual ~GraphicsView();
    void play();
    void pause();

protected:
    void resizeEvent(QResizeEvent *event);

};
typedef struct TCallbackParam
{
    QMutex          mutex;
    GraphicsScene   wnd;
    uchar          *pixels;

}TCallbackParam;
}
#endif // GRAPHICSVIEW_H
