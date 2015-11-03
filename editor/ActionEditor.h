#ifndef ActionEditor_H
#define ActionEditor_H

#include <QWidget>

#include "Action.h"

class QStandardItem;
class QStandardItemModel;
class QTreeView;
class QModelIndex;
class QComboBox;
class QLineEdit;
class QStackedWidget;
class QTextEdit;
class QLabel;

class ConversationData;
class ConversationDataInterface;

class ActionEditor : public QWidget { Q_OBJECT
public:
private:
    ConversationDataInterface *m_dataInterface;
    ConversationData *m_data;
    QStandardItemModel *m_model;
    QTreeView *m_actionView;
    QComboBox *m_currentType;
    QStackedWidget *m_currentStack;
    QComboBox *m_currentSpeaker;
    QTextEdit *m_currentSpeech;
    QComboBox *m_currentEmoter;
    QTextEdit *m_currentEmote;
    QLabel *m_currentJumpTarget;
    QStandardItem *m_current;
public:
    ActionEditor(ConversationDataInterface *interface, ConversationData *data,
        QStandardItemModel *model);
private slots:
    void addAction();
    void removeAction();
    void currentChanged(const QModelIndex &now, const QModelIndex &before);
    void changeType(int to);
private:
    void changeTo(QStandardItem *item);
};

#endif
