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

class ActionEditor : public QWidget { Q_OBJECT
public:
private:
    QStandardItemModel *m_model;
    QTreeView *m_actionView;
    QComboBox *m_currentType;
    QStackedWidget *m_currentStack;
    QLineEdit *m_currentSpeaker;
    QTextEdit *m_currentSpeech;
    QStandardItem *m_current;
public:
    ActionEditor(QStandardItemModel *model);
private slots:
    void addAction();
    void removeAction();
    void currentChanged(const QModelIndex &now, const QModelIndex &before);
    void changeType(int to);
private:
    void changeTo(QStandardItem *item);
};

#endif
