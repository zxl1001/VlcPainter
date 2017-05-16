#ifndef VLCPAINTER_H
#define VLCPAINTER_H

#include <vlc/vlc.h>
#include <QOpenGLWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QMutex>
#include <QRectF>
namespace VLCVideo{

class VlcPainter : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit VlcPainter(QWidget *parent = 0);
    ~VlcPainter();
    void updatePicture(const QImage &image);

    uint64_t tm() const;
    void setTm(const uint64_t &tm);

    float persent() const;
    void setPersent(float persent);

    float fps() const;
    void setFps(float fps);

signals:

public slots:
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QPixmap      m_FramePix;
    uint64_t     m_tm;
    float        m_persent;
    float        m_fps;
    QRectF       m_rect;
    bool         m_isMove;
    QPointF      m_oldPoint;
};


}
#endif // VLCPAINTER_H
