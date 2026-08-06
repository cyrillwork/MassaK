#include <iostream>

#include "mainwindow.h"

#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{    
    std::string help_str = "Ошибка. Используемые параметры ввода:\n\t--port имя порта (например ttyS4)\n\t--i путь и имя файла json (информация по весам)";

    std::string port_name;
    std::string file_name;

    QApplication app(argc, argv);       

    if(argc > 1)
    {
        if(!((argc == 3) || (argc == 5))) {
            std::cout << help_str << std::endl;
            return -1;
        }

        int shift = 1;
        while(true)
        {
            if(std::string(argv[shift]) == "--port") {
                port_name = std::string(argv[shift + 1]);
            } else if(std::string(argv[shift]) == "--i") {
                file_name = std::string(argv[shift + 1]);
            } else {
                std::cout << help_str << std::endl;
                return -1;
            }

            shift += 2;
            if(shift >= argc) {
                break;
            }
        }
    }

    if(!DEBUG_VERBOSE) {
        //no cout prints
        std::cout.rdbuf( nullptr );
    }

    MainWindow mainWindow(port_name);
    //mainWindow.setVisible(false);
    if(!file_name.empty()) {
        mainWindow.setJsonFilename(file_name);
    }

    //mainWindow.showFullScreen();

    return app.exec();
}
