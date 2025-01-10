#include "controller.h"

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

void Controller::routine()
{
    std::cout << "routine start" << std::endl;

    while (is_run) {
        std::cout << "routine alive" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }

    std::cout << "routine stop" << std::endl;
}
