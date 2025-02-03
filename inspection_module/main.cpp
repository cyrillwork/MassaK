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

    w.setWindowState(Qt::WindowFullScreen);
    //w.setWindowTitle("Test Massa K");
    //w.setWindowFlags(Qt::WindowFullscreenButtonHint);
    //w.setWindowFlags(w.windowFlags() & ~Qt::WindowCloseButtonHint);
    w.show();

    return a.exec();
}
