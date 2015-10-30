#ifndef ConversationWindow_H
#define ConversationWindow_H

#include <QMdiSubWindow>
#include <QSignalMapper>
#include <QList>

#include "ConversationView.h"
#include "ConversationObject.h"

class QPushButton;
class QSplitter;
class QXmlStreamWriter;

class ConversationWindow : public QMdiSubWindow { Q_OBJECT
private:
    enum Mode {
        SelectMode,
        NewNodeMode,
        NewLinkMode,
        NewContextMode,
        DeleteMode
    };
private:
    QSplitter *m_split;
    QWidget *m_edit;
    QWidget *m_editbar;
    QWidget *m_editarea;
    ConversationView *m_cview;
    QSignalMapper *m_modeMapper;
    QList<QPushButton *> m_toolButtons;

    Mode m_mode;

    ConversationObject *m_selectLast;
public:
    ConversationWindow();

    void save();
    void load();
private slots:
    void modeChange(int to);

    void selectObject(ConversationObject *object);
    void insertNode(QPointF where);
    void insertContext(QPointF where);
    void makeLink(ConversationObject *object);
    void deleteObject(ConversationObject *object);
};

#endif
