#ifndef PlatformWindow_H
#define PlatformWindow_H

#include <QMdiSubWindow>

#include "EditorWindow.h"

class EditorView;

class PlatformWindow : public QMdiSubWindow, public EditorWindow {
private:
    EditorView *m_eview;
public:
    PlatformWindow();

    virtual void saveTo(QFile &file);
};

#endif
