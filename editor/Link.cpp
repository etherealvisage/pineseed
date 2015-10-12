#include <QPainter>
#include <QGraphicsScene>

#include "Link.h"
#include "moc_Link.cpp"

#include "Node.h"

Link::Link(Node *from, Node *to) : m_from(from), m_to(to) {
    setZValue(-1.0);
    m_label = "Label";
}

QRectF Link::boundingRect() const {
    qreal minx = qMin(m_from->boundingRect().left() + m_from->pos().x(),
        m_to->boundingRect().left() + m_to->pos().x());
    qreal maxx = qMax(m_from->boundingRect().right() + m_from->pos().x(),
        m_to->boundingRect().right() + m_to->pos().x());
    qreal miny = qMin(m_from->boundingRect().top() + m_from->pos().y(),
        m_to->boundingRect().top() + m_to->pos().y());
    qreal maxy = qMax(m_from->boundingRect().bottom() + m_from->pos().y(),
        m_to->boundingRect().bottom() + m_to->pos().y());

    return QRectF(QPointF(minx, miny), QPointF(maxx, maxy)).normalized();
}

void Link::paint(QPainter *painter, const QStyleOptionGraphicsItem *style,
    QWidget *widget) {

    QPointF fromPoint, toPoint;

    calculateBestLine(fromPoint, toPoint);

    painter->setPen(QPen(QBrush(Qt::darkGreen), 3.0));
    painter->drawLine(toPoint, fromPoint);

    painter->setPen(QPen(QBrush(Qt::darkGreen), 20.0));
    painter->drawPoint(fromPoint);

    painter->setPen(Qt::lightGray);
    painter->setBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
    auto lbr = labelBoundingRect();
    painter->drawRect(lbr);
    painter->setPen(QPen(QBrush(Qt::red), 1.0));
    QFontMetricsF fm((QFont()));
    painter->drawText(lbr.center() - QPointF(fm.width(m_label)/2,-4), m_label);
}

QRectF Link::labelBoundingRect() const {
    QPointF fromPoint, toPoint;
    calculateBestLine(fromPoint, toPoint);
    QPointF lineCentre = (toPoint + fromPoint)/2.0;
    QFontMetricsF fm((QFont()));
    QRectF tbr = fm.boundingRect(m_label);

    tbr.setTopLeft(tbr.topLeft() - QPointF(4,2));
    tbr.setBottomRight(tbr.bottomRight() + QPointF(4,2));

    return QRectF(tbr.topLeft() + lineCentre - tbr.topRight()/2,
        tbr.size());
}

void Link::calculateBestLine(QPointF &from, QPointF &to) const {
    QPointF fromCentre = m_from->boundingRect().center() + m_from->pos();
    QPointF toCentre = m_to->boundingRect().center() + m_to->pos();
    QPointF fromX(m_from->boundingRect().width()/2,0);
    QPointF fromY(0,m_from->boundingRect().height()/2);
    QPointF toX(m_to->boundingRect().width()/2,0);
    QPointF toY(0,m_to->boundingRect().height()/2);

    QPointF fromPoints[4] = {fromCentre+fromX, fromCentre-fromX,
        fromCentre+fromY, fromCentre-fromY};
    QPointF toPoints[4] = {toCentre+toX, toCentre-toX,
        toCentre+toY, toCentre-toY};

    int minPair[2] = {0, 0};
    qreal minLength = (toPoints[0] - fromPoints[0]).manhattanLength();
    for(int i = 0; i < 4; i ++) {
        for(int j = 0; j < 4; j ++) {
            qreal len = (toPoints[i] - fromPoints[j]).manhattanLength();
            if(len < minLength) {
                minPair[0] = i, minPair[1] = j;
                minLength = len;
            }
        }
    }

    to = toPoints[minPair[0]];
    from = fromPoints[minPair[1]];
}
