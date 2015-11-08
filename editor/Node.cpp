#include <QDialog>
#include <QFont>
#include <QFormLayout>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
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
#include <QCheckBox>

#include "Node.h"
#include "moc_Node.cpp"

#include "Link.h"
#include "ActionEditor.h"
#include "Action.h"
#include "ConversationData.h"

Node::Node() {
    m_id = -1;
    m_selected = false;
    m_size = QSizeF(150, 100);
    m_isEntry = false;

    setFlags(ItemIsSelectable | ItemIsMovable);

    m_actionModel = new QStandardItemModel();
}

Node::~Node() {
}

QRectF Node::boundingRect() const {
    // accommodate borders
    const qreal border = 1.0;
    return QRectF(QPointF(0,0), m_size + QSizeF(border*2, border*2));
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *style,
    QWidget *widget) {

    QBrush b;
    if(m_selected) b.setColor(QColor::fromRgb(200, 200, 255));
    else if(m_isEntry) b.setColor(QColor::fromRgb(255, 224, 224));
    else b.setColor(QColor::fromRgb(240, 240, 240));
    b.setStyle(Qt::SolidPattern);
    painter->setBrush(b);
    painter->drawRect(QRectF(QPointF(0,0), m_size));

    QFontMetricsF fm((QFont()));

    qreal width = fm.width(m_label);
    painter->setPen(Qt::black);
    painter->setBrush(Qt::NoBrush);
    painter->drawText(boundingRect().center() - QPointF(width/2,0), m_label);
}

void Node::edit(ConversationDataInterface *interface,
    ConversationData *data, QFormLayout *layout) {

    if(m_id == -1) {
        m_id = data->getAvailableID();
    }

    QLineEdit *labelEdit = new QLineEdit(m_label);
    layout->addRow(tr("Label:"), labelEdit);
    labelEdit->setFocus();
    connect(labelEdit, &QLineEdit::textChanged,
        [=](const QString &label){ m_label = label; emit changed(); });

    QCheckBox *entryBox = new QCheckBox();
    entryBox->setCheckState(m_isEntry ? Qt::Checked : Qt::Unchecked);
    connect(entryBox, &QCheckBox::stateChanged,
        [=](){
            m_isEntry = entryBox->checkState() == Qt::Checked;
            update();
        });

    // make sure we don't have more than one entry node
    auto items = scene()->items();
    bool entrySet = false;
    for(auto it : items) {
        auto n = dynamic_cast<Node *>(it);
        if(!n) continue;
        if(n == this) continue;
        entrySet |= n->m_isEntry;
        if(entrySet) break;
    }

    if(entrySet) entryBox->setEnabled(false);

    layout->addRow(tr("Entry:"), entryBox);

    ActionEditor *editor = new ActionEditor(interface, data, m_actionModel);
    layout->addRow(tr(""), editor);
}

bool Node::isSelection(QPointF point) {
    return boundingRect().contains(point);
}

void Node::serialize(QXmlStreamWriter &xml) {
    xml.writeStartElement("node");

    xml.writeAttribute("id", QString().setNum(m_id));
    xml.writeAttribute("width", QString().setNum(m_size.width()));
    xml.writeAttribute("height", QString().setNum(m_size.height()));
    xml.writeAttribute("x", QString().setNum(pos().x()));
    xml.writeAttribute("y", QString().setNum(pos().y()));
    xml.writeAttribute("label", m_label);
    if(m_isEntry) xml.writeAttribute("entry", "true");

    auto root = m_actionModel->invisibleRootItem();
    for(int i = 0; i < root->rowCount(); i ++)
        Action::serialize(xml, root->child(i));

    xml.writeEndElement();
}

void Node::deserialize(QDomElement &xml,
    const QMap<int, ConversationObject *> &objs, ConversationData *data) {

    m_id = xml.attribute("id").toInt();
    m_label = xml.attribute("label");
    m_size = QSizeF(xml.attribute("width").toDouble(),
        xml.attribute("height").toDouble());
    setPos(QPointF(xml.attribute("x").toDouble(),
        xml.attribute("y").toDouble()));
    prepareGeometryChange();
    if(xml.attribute("entry") == "true")
        m_isEntry = true;

    // if there are no children, we're done here.
    if(!xml.hasChildNodes()) return;

    auto nodes = xml.childNodes();
    for(int i = 0; i < nodes.length(); i ++) {
        auto node = nodes.at(i);
        if(!node.isElement()) continue;
        auto element = node.toElement();
        if(element.tagName() != "action") continue;
        m_actionModel->invisibleRootItem()->appendRow(
            Action::deserialize(element, objs));
    }
}

void Node::visitActions(std::function<void (QStandardItem *)> visitor) {
    auto root = m_actionModel->invisibleRootItem();
    for(int i = 0; i < root->rowCount(); i ++)
        Action::walkTree(visitor, root->child(i));
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if(event->buttons() & Qt::RightButton) {
        QPointF last = mapFromScene(event->lastScenePos());
        QPointF now = mapFromScene(event->scenePos());
        QPointF delta = now-last;

        setPos(pos() + delta);

        prepareGeometryChange();
    }
}
