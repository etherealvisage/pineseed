#ifndef Node_H
#define Node_H

#include <QGraphicsObject>

class Link;
class Action;

class QListWidgetItem;

class Node : public QGraphicsObject { Q_OBJECT
private:
    QSizeF m_size;
    QString m_label;
    QVector<Link *> m_links;
    QVector<Action *> m_actions;
public:
    Node();
    ~Node();

    QVector<Action *> &actions() { return m_actions; }

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *style, QWidget *widget);

    void addLink(Link *link) { m_links.push_back(link); }
    void removeLink(Link *link);
    bool hasLink(Node *to);

    void edit(QWidget *parent);
protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif
