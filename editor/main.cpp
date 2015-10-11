#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication qapp(argc, argv);

    MainWindow mw;
    mw.show();

    return qapp.exec();
}
