#include <iostream>

#include "mainwindow.h"

#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
    std::string port_name;

    std::cout.rdbuf( nullptr );

    QApplication app(argc, argv);

    if(argc == 3 && std::string(argv[1]) == "-p") {
        port_name = std::string(argv[2]);
    }

    MainWindow mainWindow(port_name);
    mainWindow.setVisible(false);

    if(argc == 3 && std::string(argv[1]) == "-i") {
        auto _file_name = std::string(argv[2]);
        mainWindow.setJsonFilename(_file_name);
    }


    return app.exec();
}
