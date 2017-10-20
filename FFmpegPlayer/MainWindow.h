/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    MainWindow.h
 * @brief
 *******************************************************************************
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "VideoView.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void videoFrameChanged(int frameIndex);

private slots:
    void on_playBtn_clicked();

    void on_addFileBtn_clicked();

    void on_filesListWidget_doubleClicked(const QModelIndex &index);

    void on_stopBtn_clicked();

    void on_removeFileBtn_clicked();

    void on_playerProgressSlider_sliderPressed();

    void on_playerProgressSlider_sliderReleased();

    void on_playerProgressSlider_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    Video::VideoView *mPlayerView;
    bool mIsSliderDrag;
};

#endif // MAINWINDOW_H
