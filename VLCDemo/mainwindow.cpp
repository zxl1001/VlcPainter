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

void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    m_graphicsPlayer->setMediaFile(index.data().toString());
}

void MainWindow::on_stopBtn_clicked()
{
    m_graphicsPlayer->reSet();
}

void MainWindow::on_nextFrameBtn_clicked()
{
    m_graphicsPlayer->getNexFrame();
}

void MainWindow::on_saveFrameBtn_clicked()
{
    m_graphicsPlayer->saveFrame("/home/zxl/tmp");
}
