#include <QBrush>

#include "ConversationContext.h"
#include "moc_ConversationContext.cpp"

QBrush ConversationContext::deriveBrush() const {
    Qt::BrushStyle styles[] = {Qt::SolidPattern, Qt::BDiagPattern,
        Qt::FDiagPattern, Qt::Dense5Pattern};
    const int numStyles = 4;
    QColor colours[] = {Qt::red, Qt::blue, Qt::green, Qt::cyan, Qt::gray};
    const int numColours = 5;
    QBrush b;
    b.setColor(colours[m_id%numColours]);
    b.setStyle(styles[(m_id/numColours)%numStyles]);
    return b;
}
