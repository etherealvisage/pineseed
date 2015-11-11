#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSignalMapper>
#include <QShortcut>

#include "PlatformWindow.h"
#include "moc_PlatformWindow.cpp"

#include "Platform.h"
#include "EditorView.h"

PlatformWindow::PlatformWindow() : m_selectLast(nullptr) {
    auto split = new QSplitter();
    setWidget(split);

    QWidget *editWidget = new QWidget();
    split->addWidget(editWidget);
    auto editLayout = new QVBoxLayout();
    editWidget->setLayout(editLayout);

    auto modeLayout = new QHBoxLayout();
    editLayout->addLayout(modeLayout);
    editLayout->addStretch(1);

    m_modeMapper = new QSignalMapper(this);
    connect(m_modeMapper, SIGNAL(mapped(int)), this, SLOT(modeChange(int)));
    {
        auto selectPlatform = new QPushButton(tr("Select"));
        m_toolButtons.push_back(selectPlatform);
        modeLayout->addWidget(selectPlatform);
        m_modeMapper->setMapping(selectPlatform, SelectMode);
        connect(selectPlatform, SIGNAL(clicked(bool)), m_modeMapper,
            SLOT(map()));
        QShortcut *selectPlatformShortcut =
            new QShortcut(QKeySequence("Alt+1"), this);
        connect(selectPlatformShortcut, SIGNAL(activated()),
            selectPlatform, SLOT(click()));
    }
    {
        auto newPlatform = new QPushButton(tr("Add"));
        m_toolButtons.push_back(newPlatform);
        modeLayout->addWidget(newPlatform);
        m_modeMapper->setMapping(newPlatform, NewPlatformMode);
        connect(newPlatform, SIGNAL(clicked(bool)), m_modeMapper, SLOT(map()));
        QShortcut *newPlatformShortcut =
            new QShortcut(QKeySequence("Alt+2"), this);
        connect(newPlatformShortcut, SIGNAL(activated()),
            newPlatform, SLOT(click()));
    }
    {
        auto removePlatform = new QPushButton(tr("Remove"));
        m_toolButtons.push_back(removePlatform);
        modeLayout->addWidget(removePlatform);
        m_modeMapper->setMapping(removePlatform, DeleteMode);
        connect(removePlatform, SIGNAL(clicked(bool)),
            m_modeMapper, SLOT(map()));
        QShortcut *removePlatformShortcut =
            new QShortcut(QKeySequence("Alt+3"), this);
        connect(removePlatformShortcut, SIGNAL(activated()),
            removePlatform, SLOT(click()));
    }

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

    for(auto b : m_toolButtons) if(!b->isEnabled())
        b->setEnabled(true), b->setStyleSheet("");
    m_toolButtons[to]->setEnabled(false);
    m_toolButtons[to]->setStyleSheet("background-color: red");

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
    m_selectLast = object;
    if(m_selectLast) m_selectLast->select();
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
