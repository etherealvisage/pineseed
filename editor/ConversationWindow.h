#ifndef ConversationWindow_H
#define ConversationWindow_H

#include <QMdiSubWindow>

#include "ConversationView.h"

class ConversationWindow : public QMdiSubWindow { Q_OBJECT
private:
    ConversationView *m_cview;
public:
    ConversationWindow();
};

#endif
