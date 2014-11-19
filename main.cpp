#include "mainwindow.h"
#include <QApplication>
#include <QFile>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QFile file(":/new/prefix1/qss.qss");
    file.open(QFile::ReadOnly);
    a.setStyle(file.readAll());
    /*w.setWindowFlags(Qt::Tool);*/ // 不在任务栏显示
    //w.show();
    //w.hide();

    a.setQuitOnLastWindowClosed(false);
    return a.exec();
}
