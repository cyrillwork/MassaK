#include "mainwindow.h"
#include <QApplication>
#include <QStackedWidget>
#include <QPushButton>
#include <QWidget>
#include <QLayout>
//#include <iostream>
//#include <vector>
//#include <stdio.h>
//#include <signal.h>
//#include <unistd.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    //mainWindow.setWindowState(Qt::WindowFullScreen);
    mainWindow.setVisible(false);

    return app.exec();
}
