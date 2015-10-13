#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QString>
#include <QObject>
#include <QWidget>

#include "Action.h"

Action *Action::createAction(QWidget *parent) {
    Action *a = new Action();
    a->edit(parent);
    return a;
}

QString Action::description() const {
    return "Action";
}

void Action::edit(QWidget *parent) {
    QDialog dialog(parent);

    QGridLayout grid;
    dialog.setLayout(&grid);

    QPushButton okButton(QString("&Done"));
    okButton.setDefault(true);

    grid.addWidget(&okButton, 0, 0);

    dialog.exec();
}
