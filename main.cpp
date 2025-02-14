#include "mainwindow.h"

#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.setVisible(false);

    if(argc == 3 && std::string(argv[1]) == "-i") {
        auto _file_name = std::string(argv[2]);
        mainWindow.setJsonFilename(_file_name);
    }

    return app.exec();
}
