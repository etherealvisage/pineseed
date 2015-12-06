#ifndef Platform_H
#define Platform_H

#include <QSizeF>

#include "PlatformObject.h"

class Platform : public PlatformObject {
private:
    QSizeF m_size;
    bool m_selected;
    int m_id;
public:
    Platform();
    Platform(const QRectF &rect);

    virtual int id() const { return m_id; }

    virtual void select() { m_selected = true; }
    virtual void deselect() { m_selected = false; }

    virtual bool isSelection(QPointF point);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *style, QWidget *widget);

    virtual void edit(PlatformData *data, QFormLayout *layout);

    virtual void serialize(QXmlStreamWriter &xml);
    virtual void deserialize(QDomElement &xml,
        const QMap<int, PlatformObject *> &objs, PlatformData *data);
protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private:
    double snap(double v);
};

#endif
