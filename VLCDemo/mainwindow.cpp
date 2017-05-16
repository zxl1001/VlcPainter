#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
//    m_videoPlayer(Q_NULLPTR),
    m_graphicsPlayer(Q_NULLPTR),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);
//    m_videoPlayer = new VLCVideo::vlcPlayer(this);
//    ui->frame->layout()->addWidget(m_videoPlayer);

    //use the QGraphics View
    m_graphicsPlayer = new GraphicsVideo::GraphicsView(this);
    ui->frame->layout()->addWidget(m_graphicsPlayer);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_playBtn_clicked(bool checked)
{
    if(checked)
    {
        ui->playBtn->setText("Pause");
//        m_videoPlayer->play();
        m_graphicsPlayer->play();
    }
    else
    {
        ui->playBtn->setText("Play");
//        m_videoPlayer->pause();
        m_graphicsPlayer->pause();
    }
}
