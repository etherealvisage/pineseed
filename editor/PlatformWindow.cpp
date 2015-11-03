#include <QSplitter>

#include "PlatformWindow.h"
#include "moc_PlatformWindow.cpp"

#include "Platform.h"
#include "EditorView.h"

PlatformWindow::PlatformWindow() : m_selectLast(nullptr) {
    auto split = new QSplitter();
    setWidget(split);

    m_eview = new EditorView();
    split->addWidget(m_eview);
    modeChange(SelectMode);

    m_eview->scene()->addItem(new Platform(QRectF(0, 0, 100, 100)));
}

void PlatformWindow::saveTo(QFile &file) {
    
}

void PlatformWindow::modeChange(int to) {
    m_eview->disconnect(m_eview, SIGNAL(clicked(QPointF)), this, 0);
    m_eview->disconnect(m_eview, SIGNAL(selected(EditorObject *)),
        this, 0);

    switch((Mode)to) {
    case SelectMode:
        m_eview->enterSelectMode();
        connect(m_eview, SIGNAL(selected(EditorObject *)),
            this, SLOT(selectObject(EditorObject *)));
        break;
    case NewPlatformMode:
        m_eview->enterInsertMode();
        connect(m_eview, SIGNAL(clicked(QPointF)),
            this, SLOT(insertPlatform(QPointF)));
        break;
    case DeleteMode:
        m_eview->enterSelectMode();
        connect(m_eview, SIGNAL(selected(EditorObject *)),
            this, SLOT(deleteObject(EditorObject *)));
        break;
    }
}

void PlatformWindow::selectObject(EditorObject *object) {
    if(m_selectLast) m_selectLast->deselect();
    object->select();
    m_selectLast = object;
}

void PlatformWindow::insertPlatform(QPointF where) {
    QRectF rect(0, 0, 100, 100);
    rect.moveCenter(where);
    auto platform = new Platform(rect);
    m_eview->scene()->addItem(platform);

    connect(platform, SIGNAL(changed()), m_eview->viewport(), SLOT(update()));
    selectObject(platform);
    modeChange(SelectMode);
}

void PlatformWindow::deleteObject(EditorObject *object) {
    object->deleteLater();

    modeChange(SelectMode);
    selectObject(nullptr);
}
