/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    graphicsitembase.cpp
 * @brief
 *******************************************************************************
 */
#include "graphicsitembase.h"

const int MAX_ITEM_SIZE = 512;
const int MIN_ITEM_SIZE = 16;
//============================================================================//
//                                  ItemBase                                  //
//============================================================================//

GraphicsItemBase::GraphicsItemBase(int size, int x, int y) : m_size(size), m_isResizing(false)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setAcceptHoverEvents(true);
    setPos(x, y);
    m_startTime = QTime::currentTime();
}

GraphicsItemBase::~GraphicsItemBase()
{
}

QRectF GraphicsItemBase::boundingRect() const
{
    return QRectF((-m_size -10) / 2, (-m_size - 10) / 2 , m_size + 10, m_size + 10);
}

void GraphicsItemBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if (option->state & QStyle::State_Selected) {
        painter->setRenderHint(QPainter::Antialiasing, true);
        if (option->state & QStyle::State_HasFocus)
            painter->setPen(Qt::yellow);
        else
            painter->setPen(Qt::white);
        painter->drawRect(boundingRect());

        painter->drawLine((m_size+10) / 2 - 9, (m_size+10) / 2, (m_size+10) / 2, (m_size+10) / 2 - 9);
        painter->drawLine((m_size+10) / 2 - 6, (m_size+10) / 2, (m_size+10) / 2, (m_size+10) / 2 - 6);
        painter->drawLine((m_size+10) / 2 - 3, (m_size+10) / 2, (m_size+10) / 2, (m_size+10) / 2 - 3);

        painter->setRenderHint(QPainter::Antialiasing, false);
    }
}

void GraphicsItemBase::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if (!isSelected() && scene()) {
        scene()->clearSelection();
        setSelected(true);
    }

    QMenu menu;
    QAction *delAction = menu.addAction("Delete");
    QAction *newAction = menu.addAction("New");
    QAction *growAction = menu.addAction("Grow");
    QAction *shrinkAction = menu.addAction("Shrink");

    QAction *selectedAction = menu.exec(event->screenPos());

    if (selectedAction == delAction)
        deleteSelectedItems(scene());
    else if (selectedAction == newAction)
        duplicateSelectedItems(scene());
    else if (selectedAction == growAction)
        growSelectedItems(scene());
    else if (selectedAction == shrinkAction)
        shrinkSelectedItems(scene());
}

void GraphicsItemBase::duplicateSelectedItems(QGraphicsScene *scene)
{
    if (!scene)
        return;

    QList<QGraphicsItem *> selected;
    selected = scene->selectedItems();

    foreach (QGraphicsItem *item, selected) {
        GraphicsItemBase *itemBase = qgraphicsitem_cast<GraphicsItemBase *>(item);
        if (itemBase)
            scene->addItem(itemBase->createNew(itemBase->m_size, itemBase->pos().x() + itemBase->m_size, itemBase->pos().y()));
    }
}

void GraphicsItemBase::deleteSelectedItems(QGraphicsScene *scene)
{
    if (!scene)
        return;

    QList<QGraphicsItem *> selected;
    selected = scene->selectedItems();

    foreach (QGraphicsItem *item, selected) {
        GraphicsItemBase *itemBase = qgraphicsitem_cast<GraphicsItemBase *>(item);
        if (itemBase)
            delete itemBase;
    }
}

void GraphicsItemBase::growSelectedItems(QGraphicsScene *scene)
{
    if (!scene)
        return;

    QList<QGraphicsItem *> selected;
    selected = scene->selectedItems();

    foreach (QGraphicsItem *item, selected) {
        GraphicsItemBase *itemBase = qgraphicsitem_cast<GraphicsItemBase *>(item);
        if (itemBase) {
            itemBase->prepareGeometryChange();
            itemBase->m_size *= 2;
            if (itemBase->m_size > MAX_ITEM_SIZE)
                itemBase->m_size = MAX_ITEM_SIZE;
        }
    }
}

void GraphicsItemBase::shrinkSelectedItems(QGraphicsScene *scene)
{
    if (!scene)
        return;

    QList<QGraphicsItem *> selected;
    selected = scene->selectedItems();

    foreach (QGraphicsItem *item, selected) {
        GraphicsItemBase *itemBase = qgraphicsitem_cast<GraphicsItemBase *>(item);
        if (itemBase) {
            itemBase->prepareGeometryChange();
            itemBase->m_size /= 2;
            if (itemBase->m_size < MIN_ITEM_SIZE)
                itemBase->m_size = MIN_ITEM_SIZE;
        }
    }
}

void GraphicsItemBase::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_isResizing) {
        int dx = int(2.0 * event->pos().x());
        int dy = int(2.0 * event->pos().y());
        prepareGeometryChange();
        m_size = (dx > dy ? dx : dy);
        if (m_size < MIN_ITEM_SIZE)
            m_size = MIN_ITEM_SIZE;
        else if (m_size > MAX_ITEM_SIZE)
            m_size = MAX_ITEM_SIZE;
    } else {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void GraphicsItemBase::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (m_isResizing || (isInResizeArea(event->pos()) && isSelected()))
        setCursor(Qt::SizeFDiagCursor);
    else
        setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverMoveEvent(event);
}

void GraphicsItemBase::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    static qreal z = 0.0;
    setZValue(z += 1.0);
    if (event->button() == Qt::LeftButton && isInResizeArea(event->pos())) {
        m_isResizing = true;
    } else {
        QGraphicsItem::mousePressEvent(event);
    }
}

void GraphicsItemBase::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_isResizing) {
        m_isResizing = false;
    } else {
        QGraphicsItem::mouseReleaseEvent(event);
    }
}

void GraphicsItemBase::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Delete:
        deleteSelectedItems(scene());
        break;
    case Qt::Key_Insert:
        duplicateSelectedItems(scene());
        break;
    case Qt::Key_Plus:
        growSelectedItems(scene());
        break;
    case Qt::Key_Minus:
        shrinkSelectedItems(scene());
        break;
    default:
        QGraphicsItem::keyPressEvent(event);
        break;
    }
}

void GraphicsItemBase::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    prepareGeometryChange();
    m_size = int(m_size * qExp(-event->delta() / 600.0));
    if (m_size > MAX_ITEM_SIZE)
        m_size = MAX_ITEM_SIZE;
    else if (m_size < MIN_ITEM_SIZE)
        m_size = MIN_ITEM_SIZE;
}

int GraphicsItemBase::type() const
{
    return Type;
}


bool GraphicsItemBase::isInResizeArea(const QPointF &pos)
{
    return (-pos.y() < pos.x() - m_size + 9);
}
