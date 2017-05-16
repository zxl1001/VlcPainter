#include <vlc/vlc.h>
#include <QMutex>
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QTime>
#include <QDebug>
#include <QOpenGLWidget>

const int width = 1024;
const int height = 768;
libvlc_media_player_t *mediaPlayer;
class VlcPainter : public QOpenGLWidget
{

public:
    VlcPainter(QWidget *parent = 0){m_curIdx=0;}
    ~VlcPainter(){}

    void updatePicture(const QImage &image)
    {
        m_FramePix = QPixmap::fromImage(image);
        update();
        float persent = libvlc_media_player_get_position(mediaPlayer);
        float fps = libvlc_media_player_get_fps(mediaPlayer);
        m_tm = libvlc_media_player_get_time(mediaPlayer);
        //        if(persent > 0.3)
        //        {
        //            libvlc_media_player_set_position(player,0.2);
        //        }

        qDebug()<<m_tm<<persent<<fps;
    }


protected:
    void paintGL(){
        QPainter painter(this);
//        painter.setClipRegion(QRegion(10,10,500,500, QRegion::Ellipse));
        painter.drawPixmap(this->rect(), m_FramePix);
        QTime time(0,0,0);
        QFont font;
        font.setPixelSize(50);
        painter.setFont(font);
        painter.setBrush(Qt::green);
        painter.setPen(QPen(Qt::darkGray));
        painter.drawText(100,100, time.addMSecs(m_tm).toString("hh:mm:ss.zzz") /*QString::number(m_tm/1000)*/);
    }

//    void paintEvent(QPaintEvent *event)
//    {
//        QTime time(0,0,0);
//        QPainter painter(this);
//        painter.setClipRegion(QRegion(10,10,500,500, QRegion::Ellipse));
//        painter.drawPixmap(this->rect(), m_FramePix);

//        QFont font;
//        font.setPixelSize(50);
//        painter.setFont(font);
//        painter.setBrush(Qt::green);
//        painter.setPen(QPen(Qt::darkGray));
//        painter.drawText(100,100, time.addMSecs(m_tm).toString("hh:mm:ss.zzz") /*QString::number(m_tm/1000)*/);

//        QRect rect1(20, 100, 200, 80);
//        QRect rect2(20, 120, 250, 100);
//        QRegion r1(rect1);
//        QRegion r2(rect2, QRegion::Ellipse);
//        QRegion r3 = r1.intersected(r2);

//        QPainterPath path;
//        path.addRegion(r3);
//        QPen pen(QColor(255,0,0),2);
//        painter.setPen(Qt::green);
//        painter.drawRect(rect1);

//        painter.setPen(Qt::red);
//        painter.drawRect(rect2);


//        painter.setPen(pen);
//        painter.setBrush(Qt::yellow);
//        painter.drawPath(path);
//    }

private:
    QPixmap      m_FramePix;
    uint         m_curIdx;
    libvlc_time_t m_tm;
};

typedef struct TCallbackParam
{
    QMutex          mutex;
    VlcPainter      wnd;
    uchar          *pixels;
    int             dura;

}TCallbackParam;

static void *lock(void *op, void **plane)
{
    TCallbackParam *p = (TCallbackParam *)op;
    p->mutex.lock();
    *plane = p->pixels;

    return NULL;
}

static void unlock(void *op, void *pic, void *const *plane)
{
    TCallbackParam *p = (TCallbackParam *)op;
    uchar *pp = (uchar *)*plane;
    unsigned char *data = (unsigned char *)*plane;
    QImage a(data, width, height, QImage::Format_ARGB32);
    p->wnd.updatePicture(a);
    p->mutex.unlock();
}

static void display(void *op, void *pic)
{
    (void)op;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    libvlc_instance_t *instance;
    libvlc_media_t *media;
    //    libvlc_media_player_t *player;
//    const char * file = "/home/zxl/Videos/zxl/BDS-test__ch_video.m4v";
    const char * file = "/home/zxl/bds_cache/BDSR-0001_1.1/BDSR-0001__VIDEO.mp4";

    TCallbackParam *param = new TCallbackParam;
    param->pixels = new uchar[width * height * 4];
    memset(param->pixels, 0, width * height * 4);

    instance = libvlc_new(0, NULL);
    media = libvlc_media_new_path (instance, file);
    mediaPlayer = libvlc_media_player_new_from_media (media);
    libvlc_media_release (media);

    libvlc_video_set_callbacks(mediaPlayer, lock, unlock, display, param);
    libvlc_video_set_format(mediaPlayer, "RGBA", width, height, width * 4);
    libvlc_media_player_play (mediaPlayer);

    param->wnd.show();

    return a.exec();
}
