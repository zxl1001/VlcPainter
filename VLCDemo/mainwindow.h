#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "vlcplayer.h"
#include "graphicsview.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_playBtn_clicked(bool checked);

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_stopBtn_clicked();

    void on_nextFrameBtn_clicked();

    void on_saveFrameBtn_clicked();

    void on_goBtn_clicked();

private:
    Ui::MainWindow *ui;
//    VLCVideo::vlcPlayer *m_videoPlayer;
    GraphicsVideo::GraphicsView *m_graphicsPlayer;
};

#endif // MAINWINDOW_H
