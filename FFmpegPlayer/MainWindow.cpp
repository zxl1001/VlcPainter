/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    MainWindow.cpp
 * @brief
 *******************************************************************************
 */
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QUrl>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mIsSliderDrag(false)
    , mPlayerView(new Video::VideoView(this))
{
    ui->setupUi(this);
    ui->videoSceneLayout->addWidget(mPlayerView);
    connect(mPlayerView, &Video::VideoView::videoFrameChanged, this,
            &MainWindow::videoFrameChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::videoFrameChanged(int frameIndex)
{
    //    qDebug() << "curFrameIndex" << frameIndex;
    if (!mIsSliderDrag)
    {
        ui->playerProgressSlider->setValue((float)frameIndex
                                           / mPlayerView->getFrameCount()
                                           * ui->playerProgressSlider->maximum());
    }
}

void MainWindow::on_playBtn_clicked()
{
    if (!mPlayerView->isVideoAvailable())
    {
        qDebug() << "player not is available!";
        return;
    }

    if (mPlayerView->isPlaying())
    {
        mPlayerView->pause();
    }
    else
    {
        mPlayerView->play();
    }
}

void MainWindow::on_addFileBtn_clicked()
{
    QStringList fileList = QFileDialog::getOpenFileNames(
            Q_NULLPTR, tr("Add video in to play list"), "/home/zxl",
            "mp4(*.mp4);;mkv(*.mkv);;m4v(*.m4v)");

    for (QString filePath : fileList)
    {
        QFileInfo fileInf(filePath);
        QListWidgetItem* item = new QListWidgetItem(fileInf.fileName());
        item->setData(Qt::UserRole, fileInf.filePath());
        ui->filesListWidget->addItem(item);
    }
}

void MainWindow::on_filesListWidget_doubleClicked(const QModelIndex& index)
{
    QFileInfo fileInf(index.data(Qt::UserRole).toString());

    if (!fileInf.exists())
    {
        qDebug() << "file not is exists!" << fileInf.filePath();
        return;
    }

    if (mPlayerView->isReady() || mPlayerView->isPlaying()
        || mPlayerView->isPaused())
    {
        mPlayerView->uninitializeVideo();
    }
    if (!mPlayerView->initializeVideo(fileInf.filePath()))
    {
        qDebug() << "initialize video player fail!";
    }
}

void MainWindow::on_stopBtn_clicked()
{
    mPlayerView->stop();
}

void MainWindow::on_removeFileBtn_clicked()
{
    ui->filesListWidget->removeItemWidget(ui->filesListWidget->currentItem());
}

void MainWindow::on_playerProgressSlider_sliderPressed()
{
    mIsSliderDrag = true;
}

void MainWindow::on_playerProgressSlider_sliderReleased()
{
    mIsSliderDrag = false;
}

void MainWindow::on_playerProgressSlider_sliderMoved(int position)
{
    if(position == ui->playerProgressSlider->value())
    {
        return;
    }

    int frameIndex = ((float)position / ui->playerProgressSlider->maximum())
                     * mPlayerView->getFrameCount();

    mPlayerView->gotoFrame(frameIndex);
}
