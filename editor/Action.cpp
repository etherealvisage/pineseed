#include <QDialog>
#include <QGraphicsView>
#include <QGridLayout>
#include <QPushButton>
#include <QString>
#include <QObject>
#include <QWidget>

#include "Action.h"

Action *Action::createAction(QWidget *parent) {
    //Action *a = new Action();
    //a->edit(parent);
    //return a;
    return nullptr;
}

QString Action::description() const {
    return "Action";
}

void Action::edit(QGraphicsView *parent) {
    QDialog dialog(parent);

    QGridLayout grid;
    dialog.setLayout(&grid);

    QPushButton okButton(QString("&Done"));
    okButton.setDefault(true);
    grid.addWidget(&okButton, 0, 0);
    QObject::connect(&okButton, SIGNAL(clicked(bool)), &dialog, SLOT(accept()));

    dialog.exec();
}
