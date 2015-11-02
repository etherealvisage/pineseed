#ifndef EditorView_H
#define EditorView_H

#include <QGraphicsView>

class EditorObject;

class EditorView : public QGraphicsView { Q_OBJECT
private:
    enum {
        DragMode,
        SelectMode,
        InsertMode
    } m_viewMode;
private:
    QGraphicsItem *m_origin;
public:
    EditorView();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
public:
    void enterDragMode();
    void enterSelectMode();
    void enterInsertMode();
signals:
    void clicked(QPointF pos);
    void selected(EditorObject *item);
private:
    EditorObject *objectAt(QPoint uViewportPos);
};

#endif
