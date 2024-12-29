#include "mainwindow.h"
#include <QApplication>
#include <filesystem>
#include <iostream>
#include <vector>

#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void findFilesWithMask(const std::string& directory, const std::string& mask, std::vector<std::string>& result_array) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.path().filename().string().find(mask) != std::string::npos) {
            //std::cout << "Found file: " << entry.path() << std::endl;
            result_array.push_back(entry.path());
        }
    }
}

void checkCOMPorts(const std::vector<std::string>& result_array,
                   std::vector<std::string>& ports_array)
{
    for(const auto& str_port: result_array) {
        const char* _name = str_port.c_str();
        int fd = ::open(_name, O_RDONLY | O_NOCTTY | O_NDELAY);

        if (fd < 0) {
            std::cout << "IsPortAvailableByFile not open " << _name << std::endl;
            continue;
        }

        int data;
        bool result = false;

        if (ioctl(fd, TIOCMGET, &data) == 0) {
            result = true;
        }
        close(fd);

        if(!result) {
            std::cout << "IsPortAvailableByFile not ioctl " << _name << std::endl;
            continue;
        }
        ports_array.push_back(str_port);
    }
}


void find_tty_ports()
{
    std::vector<std::string> result_array;
    std::vector<std::string> ports_array;

    std::string directory = "/dev/";
    std::string mask1 = "ttyS";
    std::string mask2 = "ttyUSB";

    findFilesWithMask(directory, mask1, result_array);
    findFilesWithMask(directory, mask2, result_array);

    checkCOMPorts(result_array, ports_array);

    for(const auto& iii: ports_array) {
        std::cout << "Found file: " << iii << std::endl;
    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    find_tty_ports();

    return a.exec();
}
