#ifndef Link_H
#define Link_H

#include <QGraphicsObject>

class Node;

class Link : public QGraphicsObject { Q_OBJECT
private:
    Node *m_from, *m_to;
    QString m_label;
public:
    Link(Node *from, Node *to);

    QString label() const { return m_label; }
    void setLabel(QString label) { m_label = label; }

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *style, QWidget *widget);
};

#endif
