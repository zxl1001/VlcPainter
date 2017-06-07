/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    graphicsview.cpp
 * @brief
 *******************************************************************************
 */
#include "graphicsview.h"
#include <QGLWidget>
#include <QGLFormat>
#include <QDebug>
#include <QTime>

namespace GraphicsVideo {
extern "C"
{
static void *lock(void *op, void **plane);
static void unlock(void *op, void *pic, void *const *plane);
static void display(void *op, void *pic);
libvlc_media_player_t *mediaPlayer = Q_NULLPTR;
libvlc_instance_t *instance = Q_NULLPTR;
libvlc_media_t *media = Q_NULLPTR;
TCallbackParam *param = Q_NULLPTR;
}
static const int picPitch = 4;
static const int  picWidth = 1280;
static const int picHeight = 800;

GraphicsView::GraphicsView(QWidget *parent)
    :QGraphicsView(parent)
{
    //set the opengl widget for QGraphicsView view port;
    //    QGLWidget *pGlWidget = new QGLWidget(QGLFormat(QGL::SampleBuffers),this);
    //    pGlWidget->makeCurrent();
    //    setViewport(pGlWidget);
    //    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    //setRenderHint(QPainter::Antialiasing, false);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setDragMode(QGraphicsView::RubberBandDrag);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    param = new TCallbackParam;
    param->pixels = new uchar[picWidth * picHeight * picPitch];
    memset(param->pixels, 0, picWidth * picHeight * picPitch);
    setScene(&param->wnd);
    param->wnd.setSceneRect(0,0,1024,768);
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

GraphicsView::~GraphicsView()
{
    if(mediaPlayer != Q_NULLPTR)
    {
        libvlc_media_player_release(mediaPlayer);
        mediaPlayer = Q_NULLPTR;
    }
    if(param != Q_NULLPTR)
    {
        delete param;
        param = Q_NULLPTR;
    }
}

void GraphicsView::setMediaFile(const QString &mediaFile)
{
    if(mediaFile.isEmpty())
    {
        return;
    }
    m_mediaFile = mediaFile;
    reSet();
    instance = libvlc_new(0, Q_NULLPTR);
    media = libvlc_media_new_path (instance, m_mediaFile.toStdString().data());
    mediaPlayer = libvlc_media_player_new_from_media(media);
    libvlc_media_release (media);
    libvlc_video_set_callbacks(mediaPlayer, lock, unlock, NULL/*display*/, param);
    libvlc_video_set_format(mediaPlayer, "RGBA", picWidth, picHeight, picWidth * picPitch);
}

void GraphicsView::play()
{
    if(mediaPlayer != Q_NULLPTR)
    {
        libvlc_media_player_play(mediaPlayer);
    }
    else if(!m_mediaFile.isEmpty())
    {
        setMediaFile(m_mediaFile);
        libvlc_media_player_play(mediaPlayer);
    }
}

void GraphicsView::reSet()
{
    if(mediaPlayer != Q_NULLPTR)
    {
        //stop the media player
        if(media)
        {
            libvlc_media_player_release(mediaPlayer);
            mediaPlayer = Q_NULLPTR;
            libvlc_release(instance);
            instance = Q_NULLPTR;
        }
        m_duration = 0;
    }
}

void GraphicsView::pause()
{
    if(mediaPlayer != Q_NULLPTR)
    {
        libvlc_media_player_pause(mediaPlayer);
    }
}

void GraphicsView::changePosition(uint pos)
{
    if(mediaPlayer != Q_NULLPTR)
    {
        libvlc_media_player_set_position(mediaPlayer,(float)(pos / 1000.0));
    }
}

void GraphicsView::getNexFrame()
{
    if(mediaPlayer != Q_NULLPTR)
    {
        libvlc_media_player_next_frame(mediaPlayer);
    }
}

void GraphicsView::getPreFrame()
{
    return;
}

void GraphicsView::saveFrame(const QString &path)
{
    QDir dir(path);
    if(mediaPlayer != Q_NULLPTR && dir.exists())
    {
        libvlc_video_take_snapshot(mediaPlayer,0,path.toStdString().data(), 0, 0);
    }
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
    scene()->setSceneRect(QRect(QPoint(0,0),event->size()));
    event->accept( );
}

static void *lock(void *op, void **plane)
{
    TCallbackParam *p = (TCallbackParam *)op;
    p->mutex.lock();
    *plane = p->pixels;

    return NULL;
}

static void unlock(void *op, void *pic, void *const *plane)
{
    //    qDebug()<< "unlock" << op <<pic;
    TCallbackParam *p = (TCallbackParam *)op;
    //    uchar *pp = (uchar *)*plane;
    unsigned char *data = (unsigned char *)*plane;
    //    QImage a(data, picWidth, picWidth, QImage::Format_ARGB32);
    p->wnd.updatePicture(QImage(data, picWidth, picHeight, QImage::Format_ARGB32));
    float persent = libvlc_media_player_get_position(mediaPlayer);
    float fps = libvlc_media_player_get_fps(mediaPlayer);
    uint64_t tm = libvlc_media_player_get_time(mediaPlayer);
    if(persent > 0.95)
    {
        libvlc_media_player_set_position(mediaPlayer, 0.0);
    }
    p->wnd.setTm(tm);
    p->wnd.setPersent(persent);
    p->wnd.setFps(fps);
    p->wnd.setDuration((qint64)(libvlc_media_player_get_length(mediaPlayer)));
    p->mutex.unlock();
//        qDebug()<< persent << fps << tm <<libvlc_media_player_get_length(mediaPlayer); //ms
}

static void display(void *op, void *pic)
{
    //    qDebug()<<"display"<<op<<pic;
    (void)op;
}
//name space end
}
