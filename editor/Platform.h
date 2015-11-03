#ifndef Platform_H
#define Platform_H

#include <QSizeF>

#include "EditorObject.h"

class Platform : public EditorObject {
private:
    QSizeF m_size;
    bool m_selected;
    int m_id;
public:
    Platform(const QRectF &rect);

    virtual int id() const { return m_id; }

    virtual void select() { m_selected = true; }
    virtual void deselect() { m_selected = false; }

    virtual bool isSelection(QPointF point);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *style, QWidget *widget);
protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif
