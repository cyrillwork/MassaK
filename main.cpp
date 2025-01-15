#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <vector>

//#include "checkcomports.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Test Massa K");
    w.show();

//    {
//        CheckCOMPorts ports;
//        COMPorts array_ports;
//        ports.get_tty_ports(array_ports);
//        for(const auto& iii: array_ports) {
//            std::cout << "Found file: " << iii << std::endl;
//        }
//    }

    return a.exec();
}
