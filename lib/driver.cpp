#include "driver.h"

#include <iostream>
#include <memory>

#include "controller.h"

Driver::Driver():
    is_run{false}
{
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

void Driver::getMassa()
{
    Data data;
    Protocol::getMassa(data);
    Protocol::print(data);
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
    std::unique_ptr<Controller> controller;

    while (is_run)
    {
        std::cout << "---------------------------------" << std::endl;

        if(!controller) {
            std::cout << "make controller" << std::endl;
            controller = std::make_unique<Controller>("/dev/ttyACM0");
            std::cout << "is_init: " << controller->isInit() << std::endl;
        }

        if(controller && controller->isInit()) {
            Data data;
            Data recv_data;
            //Protocol::setTare(data);
            //Protocol::setZero(data);
            Protocol::getMassa(data);
            //Protocol::getScalePar(data);

            Protocol::print(data);
            if(controller->send(data)) {
               if(controller->read(recv_data) && Protocol::check_crc(recv_data)) {
                   ScalesParameters _temp;
                   Protocol::parseResponseGetMassa(recv_data, _temp);
                   scalesParameters = _temp;
               }
            }
        } else {
            std::cout << "make controller" << std::endl;
        }

        printScalesParameters();

        //printScalesParameters();
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }

    std::cout << "routine stop" << std::endl;
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

