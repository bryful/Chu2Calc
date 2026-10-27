#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("tyu2calc");
    QCoreApplication::setOrganizationName("bry-ful");


    MainWindow w;
    w.show();

    return a.exec();
}
