/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    vlcplayer.h
 * @brief
 *******************************************************************************
 */
#ifndef VLCPLAYER_H
#define VLCPLAYER_H
#include "vlcpainter.h"
#include <QWidget>

namespace Ui {
class vlcPlayer;
}
namespace VLCVideo{
class vlcPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit vlcPlayer(QWidget *parent = 0);
    ~vlcPlayer();
    void play();
    void pause();

private:
    Ui::vlcPlayer *ui;
};
typedef struct TCallbackParam
{
    QMutex          mutex;
    VlcPainter     wnd;
    uchar          *pixels;

}TCallbackParam;
}
#endif // VLCPLAYER_H
