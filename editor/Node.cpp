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

void Node::deserialize(QXmlStreamReader &xml,
    const QMap<int, ConversationObject *> &items) {

    m_size = QSizeF(xml.attributes().value("width").toDouble(),
        xml.attributes().value("height").toDouble());
    setPos(QPointF(xml.attributes().value("x").toDouble(),
        xml.attributes().value("y").toDouble()));

    m_label = xml.attributes().value("label").toString();

    QStandardItem *action;
    xml.readNext();
    while((action = actionDeserializeHelper(xml, items))) {
        m_actionModel->invisibleRootItem()->appendRow(action);
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

QStandardItem *Node::actionDeserializeHelper(QXmlStreamReader &xml, 
    const QMap<int, ConversationObject *> &items) {

    qDebug("actionDeserializeHelper called!");
    if(xml.isStartElement() && xml.name() != "action") return nullptr;
    qDebug("actionDeserializeHelper called, actual action found!");

    // assumes that the <action> begin token has been read

    auto action = new QStandardItem;

    action->setData(xml.attributes().value("type").toInt(),
        ActionEditor::TypeData);
    action->setData(xml.attributes().value("speech").toString(),
        ActionEditor::SpeechData);
    ActionEditor::updateActionTitle(action);

    qDebug("Parsing next...");
    while(true) {
        auto t = xml.readNext();
        qDebug("Read next token.");
        if(t == QXmlStreamReader::StartElement) {
            qDebug("Start element! Recursing...");
            auto i = actionDeserializeHelper(xml, items);
            action->appendRow(i);
        }
        else if(t == QXmlStreamReader::EndElement) {
            qDebug("End element.");
            break;
        }
    }

    qDebug("Finishing!");

    return action;
}
