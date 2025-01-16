#pragma once

#include <string>
#include <vector>

#include "aixlog.hpp"

using COMPorts = std::vector<std::string>;

class CheckCOMPorts
{
public:
    CheckCOMPorts();
    void get_tty_ports(COMPorts& tty_ports);

private:
    void checkCOMPorts(const COMPorts& result_array, COMPorts& ports_array);
    void findFilesWithMask(const std::string& directory, const std::string& mask,
                           COMPorts& result_array);
};

