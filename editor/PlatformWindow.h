#ifndef PlatformWindow_H
#define PlatformWindow_H

#include <QMdiSubWindow>

#include "EditorWindow.h"

class EditorView;
class EditorObject;

class PlatformWindow : public QMdiSubWindow, public EditorWindow { Q_OBJECT
private:
    enum Mode {
        // mode 0
        SelectMode,
        // button modes
        NewPlatformMode,
        DeleteMode
    };
private:
    EditorView *m_eview;

    Mode m_mode;

    EditorObject *m_selectLast;
public:
    PlatformWindow();

    virtual void saveTo(QFile &file);
private slots:
    void modeChange(int to);

    void selectObject(EditorObject *object);
    void insertPlatform(QPointF where);
    void deleteObject(EditorObject *object);
};

#endif
