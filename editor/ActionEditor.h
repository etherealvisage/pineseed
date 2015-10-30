#ifndef ActionEditor_H
#define ActionEditor_H

#include <QWidget>

class QStandardItem;
class QStandardItemModel;
class QTreeView;
class QModelIndex;
class QComboBox;
class QLineEdit;
class QStackedWidget;

class ActionEditor : public QWidget { Q_OBJECT
public:
    enum ActionType {
        Empty,
        Speech,
        Emote,
        Sequence,
        Concurrent,
        Conditional,
        Jump
    };
    enum ItemData {
        TypeData = 0x100,
        SpeechData
    };
private:
    QStandardItemModel *m_model;
    QTreeView *m_actionView;
    QComboBox *m_currentType;
    QStackedWidget *m_currentStack;
    QLineEdit *m_currentSpeech;
    QStandardItem *m_current;
public:
    ActionEditor(QStandardItemModel *model);
private slots:
    void addAction();
    void removeAction();
    void currentChanged(const QModelIndex &now, const QModelIndex &before);
    void changeType(int to);
public:
    static void updateActionTitle(QStandardItem *item);
private:
    void changeTo(QStandardItem *item);
};

#endif
