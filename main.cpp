#include "mainwindow.h"
#include <QApplication>
#include "popup_window.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName(QString("nyhls Applications"));
    a.setApplicationName(QString("DOMA"));
    MainWindow w;
    w.show();

    return a.exec();
}
