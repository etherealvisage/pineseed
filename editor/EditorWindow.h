#ifndef EditorWindow_H
#define EditorWindow_H

class QFile;

class EditorWindow {
public:
    virtual ~EditorWindow() {}
    virtual void saveTo(QFile &file) = 0;
    virtual void load() = 0;
};

#endif
