#include "PlatformWindow.h"
#include "EditorView.h"

PlatformWindow::PlatformWindow() {
    m_eview = new EditorView();
    setWidget(m_eview);
}

void PlatformWindow::saveTo(QFile &file) {
    
}
