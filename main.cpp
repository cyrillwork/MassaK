#include "mainwindow.h"
#include <QApplication>
#include <filesystem>
#include <iostream>

void findFilesWithMask(const std::string& directory, const std::string& mask) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().filename().string().find(mask) != std::string::npos) {
            std::cout << "Found file: " << entry.path() << std::endl;
        }
    }
}

void find_tty_ports()
{
    std::string directory = "/dev/";
    std::string mask = "tty";

    findFilesWithMask(directory, mask);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    find_tty_ports();

    return a.exec();
}
