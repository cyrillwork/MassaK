#include "driver.h"

#include <iostream>
#include <memory>

#include "controller.h"
#include "checkcomports.h"

std::unique_ptr<Controller> controller;

Driver::Driver():
    is_run{false}
{
    start();
}

Driver::~Driver()
{
    stop();
}

void Driver::start()
{
    if(main_thread) {
        stop();
    }
    is_run = true;
    main_thread = std::make_unique<std::thread>(&Driver::routine, this);
}

void Driver::stop()
{
    if(main_thread && main_thread->joinable()) {
        is_run = false;
        main_thread->join();
        main_thread.reset();
    }
}

bool Driver::getMassa()
{
    bool result = false;

    if(!(controller && controller->isInit())) {
        std::cout << "Driver::getMassa controller not init" << std::endl;
        return result;
    }

    Data data;
    Data recv_data;
    Protocol::getMassa(data);
    //Protocol::print(data);

    if(controller->send(data)) {
       if(controller->read(recv_data) && Protocol::check_crc(recv_data)) {
           ScalesParameters _temp;
           Protocol::parseResponseGetMassa(recv_data, _temp);
           scalesParameters = _temp;
           result = true;
       }
    }

    if(!result) {
        resetScaleParameters();
    }

    return result;
}

void Driver::setZero()
{
    Data data;
    Protocol::setZero(data);
    Protocol::print(data);
}

void Driver::setTare()
{
//    Data data;
//    Protocol::setTare(data);
//    Protocol::print(data);

    Protocol::test_crc();
}

void Driver::routine()
{
    std::cout << "routine start" << std::endl;
    COMPorts array_ports;

    while (is_run)
    {
        std::cout << "---------------------------------" << std::endl;        
        if(array_ports.empty()) {
            CheckCOMPorts ports;
            ports.get_tty_ports(array_ports);
            for(const auto& iii: array_ports) {
                std::cout << "Found port: " << iii << std::endl;
            }
            std::cout << "---------------------------------" << std::endl;
        }

        if(!controller && !array_ports.empty()) {
            std::cout << "make controller port: " << array_ports.back() << std::endl;
            controller = std::make_unique<Controller>(array_ports.back());
            std::cout << "is_init: " << controller->isInit() << std::endl;
            if(!controller->isInit()) {
                array_ports.pop_back();
                controller.reset();
                controller = nullptr;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    std::cout << "routine stop" << std::endl;
}

void Driver::resetScaleParameters()
{
    scalesParameters.connection     = false;
    scalesParameters.condition      = false;
    scalesParameters.weight         = 0;
    scalesParameters.weight_stable  = false;
    scalesParameters.weight_overmax = false;
    scalesParameters.weight_net     = false;
    scalesParameters.weight_zero    = false;
}

const ScalesParameters& Driver::getScalesParameters() const
{
    return scalesParameters;
}

void Driver::printScalesParameters() const
{
    std::cout << "connection: " << scalesParameters.connection << std::endl;
    std::cout << "condition: "  << scalesParameters.condition << std::endl;
    std::cout << std::dec << "weight: "     << (int)scalesParameters.weight << std::endl;

    std::cout << "weight_stable: "  << scalesParameters.weight_stable << std::endl;
    std::cout << "weight_overmax: " << scalesParameters.weight_overmax << std::endl;
    std::cout << "weight_net: "     << scalesParameters.weight_net << std::endl;
    std::cout << "weight_zero: "    << scalesParameters.weight_zero << std::endl;
}

