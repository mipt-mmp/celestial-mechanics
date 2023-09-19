#include "mainwindow.hpp"

#include <QApplication>
#include <QDebug>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /* After creating QApplication instance */
    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
    qDebug() << "I wanna it work\n";
    MainWindow w;
    w.show();
    return a.exec();
}
