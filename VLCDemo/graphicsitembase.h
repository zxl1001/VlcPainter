/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    graphicsitembase.h
 * @brief
 *******************************************************************************
 */
#ifndef GRAPHICSITEMBASE_H
#define GRAPHICSITEMBASE_H

#include <QtWidgets>
#include <QTime>

class GraphicsItemBase : public QGraphicsItem
{
public:
    enum { Type = UserType + 1 };

    GraphicsItemBase(int size, int x, int y);
    virtual ~GraphicsItemBase();
    virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
protected:
    virtual GraphicsItemBase *createNew(int size, int x, int y) = 0;
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    virtual void wheelEvent(QGraphicsSceneWheelEvent *event) Q_DECL_OVERRIDE;
    virtual int type() const Q_DECL_OVERRIDE;
    bool isInResizeArea(const QPointF &pos);

    static void duplicateSelectedItems(QGraphicsScene *scene);
    static void deleteSelectedItems(QGraphicsScene *scene);
    static void growSelectedItems(QGraphicsScene *scene);
    static void shrinkSelectedItems(QGraphicsScene *scene);

    int m_size;
    QTime m_startTime;
    bool m_isResizing;
};

#endif // GRAPHICSITEMBASE_H
