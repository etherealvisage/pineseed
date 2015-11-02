#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>
#include <QMdiArea>

class MainWindow : public QMainWindow { Q_OBJECT
private:
    QMdiArea *m_mdi;
public:
    MainWindow();
    virtual ~MainWindow() {}
private slots:
    void newConversationWindow();
    void newPlatformWindow();
    void saveCurrent();
    void loadCurrent();
    void autoSave();
};

#endif
