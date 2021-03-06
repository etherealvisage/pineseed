#ifndef PlatformWindow_H
#define PlatformWindow_H

#include <QMdiSubWindow>

#include "EditorWindow.h"

class QPushButton;
class QSignalMapper;

class EditorView;
class EditorObject;
class PlatformData;

class PlatformWindow : public QMdiSubWindow, public EditorWindow { Q_OBJECT
private:
    enum Mode {
        SelectMode,
        NewPlatformMode,
        DeleteMode
    };
private:
    PlatformData *m_data;
    EditorView *m_eview;
    QWidget *m_editarea;
    QList<QPushButton *> m_toolButtons;
    QSignalMapper *m_modeMapper;

    Mode m_mode;

    EditorObject *m_selectLast;
public:
    PlatformWindow();

    virtual void saveTo(QFile &file);
    virtual void load();
private slots:
    void modeChange(int to);

    void selectObject(EditorObject *object);
    void insertPlatform(QPointF where);
    void deleteObject(EditorObject *object);
};

#endif
