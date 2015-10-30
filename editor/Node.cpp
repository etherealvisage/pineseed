#include <QDialog>
#include <QFont>
#include <QFormLayout>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsView>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMenu>
#include <QPainter>
#include <QPushButton>
#include <QStandardItemModel>
#include <QTreeView>
#include <QStackedWidget>
#include <QComboBox>
#include <QXmlStreamWriter>
#include <QDomElement>

#include "Node.h"
#include "moc_Node.cpp"

#include "Link.h"
#include "ActionEditor.h"

Node::Node() {
    m_size = QSizeF(150, 100);

    setFlags(ItemIsSelectable | ItemIsMovable);

    m_actionModel = new QStandardItemModel();
}

Node::~Node() {
    for(auto link : m_links) delete link;
}

QRectF Node::boundingRect() const {
    // accommodate borders
    const qreal border = 1.0;
    return QRectF(QPointF(0,0), m_size + QSizeF(border*2, border*2));
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *style,
    QWidget *widget) {

    QBrush b;
    b.setColor(QColor::fromRgb(240, 240, 240));
    b.setStyle(Qt::SolidPattern);
    painter->setBrush(b);
    painter->drawRect(QRectF(QPointF(0,0), m_size));

    QFontMetricsF fm((QFont()));

    qreal width = fm.width(m_label);
    painter->setPen(Qt::black);
    painter->setBrush(Qt::NoBrush);
    painter->drawText(boundingRect().center() - QPointF(width/2,0), m_label);
}

void Node::edit(QFormLayout *layout) {
    QLineEdit *labelEdit = new QLineEdit(m_label);
    layout->addRow(tr("Label:"), labelEdit);
    labelEdit->setFocus();
    connect(labelEdit, &QLineEdit::textChanged,
        [=](const QString &label){ m_label = label; emit changed(); });

    ActionEditor *editor = new ActionEditor(m_actionModel);
    layout->addRow(tr(""), editor);
}

bool Node::isSelection(QPointF point) {
    return boundingRect().contains(point);
}

void Node::serialize(QXmlStreamWriter &xml,
    const QMap<ConversationObject *, int> &itemID) {

    xml.writeStartElement("node");

    xml.writeAttribute("id", QString().setNum(itemID[this]));
    xml.writeAttribute("width", QString().setNum(m_size.width()));
    xml.writeAttribute("height", QString().setNum(m_size.height()));
    xml.writeAttribute("x", QString().setNum(pos().x()));
    xml.writeAttribute("y", QString().setNum(pos().y()));
    xml.writeAttribute("label", m_label);

    auto root = m_actionModel->invisibleRootItem();
    for(int i = 0; i < root->rowCount(); i ++)
        actionSerializeHelper(xml, itemID, root->child(i));

    xml.writeEndElement();
}

void Node::deserialize(QDomElement &xml,
    const QMap<int, ConversationObject *> &objs) {

    m_label = xml.attribute("label");
    m_size = QSizeF(xml.attribute("width").toDouble(),
        xml.attribute("height").toDouble());
    setPos(QPointF(xml.attribute("x").toDouble(),
        xml.attribute("y").toDouble()));
    prepareGeometryChange();

    // if there are no children, we're done here.
    if(!xml.hasChildNodes()) return;

    auto nodes = xml.childNodes();
    for(int i = 0; i < nodes.length(); i ++) {
        auto node = nodes.at(i);
        if(!node.isElement()) continue;
        auto element = node.toElement();
        if(element.tagName() != "action") continue;
        m_actionModel->invisibleRootItem()->appendRow(
            actionDeserializeHelper(element, objs));
    }
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton) {
        QPointF last = mapFromScene(event->lastScenePos());
        QPointF now = mapFromScene(event->scenePos());
        QPointF delta = now-last;

        setPos(pos() + delta);

        for(auto view : scene()->views()) {
            view->repaint();
        }
    }
}

void Node::actionSerializeHelper(QXmlStreamWriter &xml, 
    const QMap<ConversationObject *, int> &itemID, QStandardItem *action) {

    xml.writeStartElement("action");

    xml.writeAttribute("type",
        action->data(ActionEditor::TypeData).toString());
    xml.writeAttribute("speech",
        action->data(ActionEditor::SpeechData).toString());

    for(int i = 0; i < action->rowCount(); i ++) {
        actionSerializeHelper(xml, itemID, action->child(i));
    }

    xml.writeEndElement();
}

QStandardItem *Node::actionDeserializeHelper(QDomElement &xml, 
        const QMap<int, ConversationObject *> &objs) {

    auto action = new QStandardItem();

    action->setData(xml.attribute("type").toInt(), ActionEditor::TypeData);
    action->setData(xml.attribute("speech"), ActionEditor::SpeechData);
    ActionEditor::updateActionTitle(action);

    auto nodes = xml.childNodes();
    for(int i = 0; i < nodes.size(); i ++) {
        auto node = nodes.at(i);
        if(!node.isElement()) continue;
        auto element = node.toElement();
        if(element.tagName() != "action") continue;

        action->appendRow(actionDeserializeHelper(element, objs));
    }

    return action;
}
