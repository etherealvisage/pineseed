#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>
#include <QMdiArea>

class QMenu;

class MainWindow : public QMainWindow { Q_OBJECT
private:
    QMdiArea *m_mdi;
    QMenu *m_conversationMenu;
    QMenu *m_platformMenu;
public:
    MainWindow();
    virtual ~MainWindow() {}
private slots:
    void newConversationWindow();
    void newPlatformWindow();
    void saveCurrent();
    void loadCurrent();
    void autoSave();
    void subwindowChange(QMdiSubWindow *window);
};

#endif
