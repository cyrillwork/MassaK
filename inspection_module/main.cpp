#include "mainwindow.h"
#include <QApplication>

//#include <iostream>
//#include <vector>
//#include <stdio.h>
//#include <signal.h>
//#include <unistd.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Test Massa K");
    w.show();

    return a.exec();
}
