#include "vlcpainter.h"
#include <QTime>
namespace VLCVideo{
VlcPainter::VlcPainter(QWidget *parent)
    : QOpenGLWidget(parent),
      m_isMove(false),
      m_oldPoint(0,0)
{
    m_persent = 0;
    m_fps     = 0;
    m_tm      = 0;
//    QSurfaceFormat format;
//    format.setDepthBufferSize(24);
//    format.setStencilBufferSize(8);
//    format.setVersion(3, 2);
//    format.setProfile(QSurfaceFormat::CoreProfile);
//    setFormat(format); // must be called before the widget or its parent window gets shown

    m_rect.setRect(200,200,100,100);
}

VlcPainter::~VlcPainter()
{

}

void VlcPainter::updatePicture(const QImage &image)
{
    m_FramePix = QPixmap::fromImage(image);
    update();

}

void VlcPainter::paintGL()
{
    QPainter painter(this);
    painter.drawPixmap(this->rect(), m_FramePix);

    //draw the attribute
    QRect attrRect(20,20,300,100);
    painter.save();
    painter.setBrush(QBrush(QColor(0,0,0,80)));
    painter.setPen(Qt::white);
    painter.drawRoundedRect(attrRect.marginsAdded(QMargins(10,10,0,0)),5,5);
    painter.restore();

    QFont font;
    QTime time(0,0,0);
    painter.save();
    font.setPixelSize(24);
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(attrRect,
                     QString("Persent: %1\nFps: %2\nPosition: %3")
                     .arg(m_persent).arg(m_fps)
                     .arg(time.addMSecs(m_tm).toString("hh:mm:ss.zzz"))
                     );
    painter.restore();

    //check and the draw the
    painter.save();
    painter.setPen(QPen(Qt::yellow, 3));
    painter.drawRect(m_rect);
    painter.restore();

}

void VlcPainter::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(m_rect.contains(event->pos()))
        {
            m_isMove = true;
            m_oldPoint = QPoint(event->pos().x()- m_rect.x(), event->pos().y() - m_rect.y());
        }
    }

}

void VlcPainter::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isMove)
    {
        m_rect.moveTo(event->pos() - m_oldPoint);
    }
}

void VlcPainter::mouseReleaseEvent(QMouseEvent *event)
{
    m_isMove = false;
}

float VlcPainter::fps() const
{
    return m_fps;
}

void VlcPainter::setFps(float fps)
{
    m_fps = fps;
}

void VlcPainter::initializeGL()
{

}

void VlcPainter::resizeGL(int w, int h)
{
    if(h == 0)
    {
        h = 1;
    }
}

float VlcPainter::persent() const
{
    return m_persent;
}

void VlcPainter::setPersent(float persent)
{
    m_persent = persent;
}

uint64_t VlcPainter::tm() const
{
    return m_tm;
}

void VlcPainter::setTm(const uint64_t &tm)
{
    m_tm = tm;
}
//name space
}
