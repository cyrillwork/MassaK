#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void setDaemon()
{
    for (int i=0; i<20; i++) {
        ::signal(i, SIG_IGN);
    }
    ::setsid();
}

void OnExit(int /*sig*/)
{
    printf("OnExit got signal");
    ::exit(0);
}

int main(int argc, char *argv[])
{
    //setDaemon();
    //::signal(SIGTERM, OnExit);

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Test Massa K");
    w.show();

    return a.exec();
}
