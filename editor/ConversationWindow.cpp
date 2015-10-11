#include <QLabel>
#include <QMenu>
#include <QAction>

#include "ConversationWindow.h"
#include "moc_ConversationWindow.cpp"

#include "Node.h"

ConversationWindow::ConversationWindow() {
    m_cview = new ConversationView();
    setWidget(m_cview);
}
