#include <QMenuBar>
#include <QLabel>
#include <QFile>
#include <QFileDialog>
#include <QTimer>

#include "MainWindow.h"
#include "moc_MainWindow.cpp"

#include "ConversationWindow.h"
#include "PlatformWindow.h"

MainWindow::MainWindow() {
    this->setWindowTitle(tr("Pineseed editor"));

    auto file = menuBar()->addMenu(tr("&File"));
    file->addAction(tr("&New conversation"), this, SLOT(newConversationWindow()));
    file->addAction(tr("New &platform level"), this,
        SLOT(newPlatformWindow()));
    file->addAction(tr("&Save"), this, SLOT(saveCurrent()));
    file->addAction(tr("&Load"), this, SLOT(loadCurrent()));
    file->addAction(tr("E&xit"), this, SLOT(close()));

    m_mdi = new QMdiArea();
    setCentralWidget(m_mdi);

    m_mdi->setViewMode(QMdiArea::TabbedView);

    QTimer *autosaveTimer = new QTimer(this);
    autosaveTimer->setSingleShot(false);
    autosaveTimer->setInterval(60 * 1000); // once per minute
    connect(autosaveTimer, SIGNAL(timeout()), this, SLOT(autoSave()));
    autosaveTimer->start();
}

void MainWindow::newConversationWindow() {
    auto cw = new ConversationWindow();
    m_mdi->addSubWindow(cw);
    cw->setAttribute(Qt::WA_DeleteOnClose);

    cw->show();
}

void MainWindow::newPlatformWindow() {
    auto pw = new PlatformWindow();
    m_mdi->addSubWindow(pw);
    pw->setAttribute(Qt::WA_DeleteOnClose);

    pw->show();
}

void MainWindow::saveCurrent() {
    auto ew = dynamic_cast<EditorWindow *>(m_mdi->activeSubWindow());
    if(!ew) return;

    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save ..."));
    QFile file(filename);
    if(!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) return;

    ew->saveTo(file);
}

void MainWindow::loadCurrent() {
    newConversationWindow();

    auto cw = dynamic_cast<ConversationWindow *>(m_mdi->activeSubWindow());
    if(!cw) return;

    cw->load();
}

void MainWindow::autoSave() {
    auto ew = dynamic_cast<EditorWindow *>(m_mdi->activeSubWindow());
    if(!ew) return;

    QFile file("autosave.xml");
    if(!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) return;

    ew->saveTo(file);
}
