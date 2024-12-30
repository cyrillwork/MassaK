#include "checkcomports.h"

#include <filesystem>

#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <iostream>

CheckCOMPorts::CheckCOMPorts()
{
}

void CheckCOMPorts::findFilesWithMask(const std::string& directory, const std::string& mask, COMPorts& result_array)
{
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.path().filename().string().find(mask) != std::string::npos) {
            //std::cout << "Found file: " << entry.path() << std::endl;
            result_array.push_back(entry.path());
        }
    }
}

void CheckCOMPorts::checkCOMPorts(const COMPorts& result_array, COMPorts& ports_array)
{
    for(const auto& str_port: result_array)
    {
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


void CheckCOMPorts::get_tty_ports(COMPorts& tty_ports)
{
    COMPorts mask_array;

    tty_ports.clear();

    std::string directory = "/dev/";
    std::string mask1 = "ttyS";
    std::string mask2 = "ttyUSB";

    findFilesWithMask(directory, mask1, mask_array);
    findFilesWithMask(directory, mask2, mask_array);

    checkCOMPorts(mask_array, tty_ports);
}

