#include "mainwindow.h"
#include <QApplication>
#include<QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    printf("MCTS AI Console\n");
    fflush(stdout);
    QIcon icon=QIcon(":/goBang.ico");
    w.setWindowIcon(icon);
    return a.exec();
}
