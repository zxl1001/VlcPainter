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
    void reSet();
    void pause();
    void changePosition(uint pos);
    void getNexFrame();
    void getPreFrame();
    void saveFrame(const QString &path);
    void setMediaFile(const QString &mediaFile);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    static void *lock(void *op, void **plane);
    static void unlock(void *op, void *pic, void *const *plane);
    static void display(void *op, void *pic);
private:
    QString m_mediaFile; //media file
    qint64 m_duration;
    int m_media_fps; //frame/s


};
typedef struct TCallbackParam
{
    QMutex          mutex;
    GraphicsScene   wnd;
    uchar          *pixels;

}TCallbackParam;
}
#endif // GRAPHICSVIEW_H
