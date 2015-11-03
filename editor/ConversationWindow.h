#ifndef ConversationWindow_H
#define ConversationWindow_H

#include <functional>

#include <QMdiSubWindow>
#include <QSignalMapper>
#include <QList>

#include "ConversationData.h"
#include "ConversationObject.h"
#include "EditorView.h"
#include "EditorWindow.h"

class QFile;
class QPushButton;
class QSplitter;
class QXmlStreamWriter;

class ConversationSimulation;

class ConversationWindow : public QMdiSubWindow, public EditorWindow { Q_OBJECT
private:
    enum Mode {
        // mode 0
        SelectMode,
        // button modes
        NewNodeMode,
        NewLinkMode,
        NewContextMode,
        DeleteMode,
        // extra non-button modes
        SelectOneMode
    };
private:
    ConversationDataInterface *m_dataInterface;
    ConversationData *m_data;
    QSplitter *m_split;
    QWidget *m_edit;
    QWidget *m_editbar;
    QWidget *m_editarea;
    QPushButton *m_simbutton;
    EditorView *m_eview;
    ConversationSimulation *m_sim;
    QSignalMapper *m_modeMapper;
    QList<QPushButton *> m_toolButtons;

    Mode m_mode;

    ConversationObject *m_selectLast;
    std::function<bool (ConversationObject *)> m_selectOneFilter;
    std::function<void (ConversationObject *)> m_selectOneCallback;
public:
    ConversationWindow();

    void saveTo(QFile &file);
    void load();
    void doWordCount();
private slots:
    void modeChange(int to);

    void selectObject(EditorObject *eobject);
    void insertNode(QPointF where);
    void insertContext(QPointF where);
    void makeLink(EditorObject *object);
    void deleteObject(EditorObject *object);
    void selectOne(EditorObject *eobject);
    void beginSimulation();
};

#endif
