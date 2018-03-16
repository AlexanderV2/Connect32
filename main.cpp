#include "mainwindow.h"
#include <QApplication>
#include "messagewnd.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));

    MainWindow w;
#ifndef Q_OS_ANDROID
    QApplication::setQuitOnLastWindowClosed(false);
#else
    w.show();
#endif

    return a.exec();
}
