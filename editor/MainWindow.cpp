#include <QMenuBar>
#include <QLabel>

#include "MainWindow.h"
#include "moc_MainWindow.cpp"

#include "ConversationWindow.h"

MainWindow::MainWindow() {
    this->setWindowTitle(tr("Pineseed editor"));

    auto file = menuBar()->addMenu(tr("&File"));
    file->addAction(tr("&New"), this, SLOT(newConversationWindow()));
    file->addAction(tr("E&xit"), this, SLOT(close()));

    m_mdi = new QMdiArea();
    setCentralWidget(m_mdi);

    m_mdi->setViewMode(QMdiArea::TabbedView);
}

void MainWindow::newConversationWindow() {
    auto cw = new ConversationWindow();
    m_mdi->addSubWindow(cw);
    cw->setAttribute(Qt::WA_DeleteOnClose);

    cw->show();
}
