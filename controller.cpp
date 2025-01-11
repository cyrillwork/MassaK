#include "controller.h"
#include "protocol.h"

#include <iostream>

Controller::Controller():
    is_run{false}
{
}

Controller::~Controller()
{
    stop();
}

void Controller::start()
{
    if(main_thread) {
        stop();
    }
    is_run = true;
    main_thread = std::make_unique<std::thread>(&Controller::routine, this);
}

void Controller::stop()
{
    if(main_thread && main_thread->joinable()) {
        is_run = false;
        main_thread->join();
        main_thread.reset();
    }
}

void Controller::getMassa()
{
    Data data;
    Protocol::getMassa(data);
    Protocol::print(data);
}

void Controller::setZero()
{
    Data data;
    Protocol::setZero(data);
    Protocol::print(data);
}

void Controller::setTare()
{
    Data data;
    Protocol::setTare(data);
    Protocol::print(data);
}

void Controller::routine()
{
    std::cout << "routine start" << std::endl;

    while (is_run) {
        std::cout << "routine alive" << std::endl;
        printScalesParameters();
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }

    std::cout << "routine stop" << std::endl;
}

const ScalesParameters& Controller::getScalesParameters() const
{
    return scalesParameters;
}

void Controller::printScalesParameters() const
{
    std::cout << "connection: " << scalesParameters.connection << std::endl;
    std::cout << "condition: "  << scalesParameters.condition << std::endl;
    std::cout << "weight: "     << scalesParameters.weight << std::endl;

    std::cout << "weight_stable: "  << scalesParameters.weight_stable << std::endl;
    std::cout << "weight_overmax: " << scalesParameters.weight_overmax << std::endl;
    std::cout << "weight_net: "     << scalesParameters.weight_net << std::endl;
    std::cout << "weight_zero: "    << scalesParameters.weight_zero << std::endl;
}
