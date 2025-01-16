#include "driver.h"

#include <iostream>
#include <memory>

#include "controller.h"
#include "checkcomports.h"

std::unique_ptr<Controller> controller;

Driver::Driver():
    is_run{false}
  , need_getMassa{false}
  , need_setZero{false}
  , need_setTare{false}
{
    AixLog::Log::init( { std::make_shared<AixLog::SinkFile>(AixLog::Severity::DataCapture, "/tmp/libMassaK.log") } );
    LOG(INFO) << "Driver start" << "\n";
    start();
}

Driver::~Driver()
{
    LOG(INFO) << "Driver stop" << "\n";
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

bool Driver::getScalePar()
{
    bool result = false;

    if(!(controller && controller->isInit())) {
        LOG(INFO) << "Driver::getScalePar controller not init" << std::endl;
        return result;
    }

    Data data;
    Data recv_data;
    Protocol::getScalePar(data);

    if(controller->send(data)) {
       if(controller->read(recv_data) && Protocol::check_crc(recv_data)) {
           result = Protocol::parseResponseGetScalePar(recv_data);
       }
    }

    if(!result) {
        LOG(INFO) << "Driver::getScalePar error parse" << std::endl;
    }

    return result;
}

void Driver::getMassa()
{
    need_getMassa = true;
}

bool Driver::m_getMassa()
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
    need_setZero = true;
}

void Driver::m_setZero()
{
    Data data;
    Protocol::setZero(data);
    Protocol::print(data);
}

void Driver::setTare()
{
    need_setTare = true;
}

void Driver::m_setTare()
{
    Data data;
    Protocol::setTare(data);
    Protocol::print(data);
}

void Driver::routine()
{
    LOG(INFO) << "routine start" << std::endl;
    COMPorts array_ports;

    while (is_run)
    {
        //std::cout << "---------------------------------" << std::endl;
        if(array_ports.empty()) {
            CheckCOMPorts ports;
            ports.get_tty_ports(array_ports);
            for(const auto& iii: array_ports) {
                LOG(INFO) << "Found port: " << iii << std::endl;
            }
            LOG(INFO) << "---------------------------------" << std::endl;
        }

        if(!controller && !array_ports.empty()) {
            LOG(INFO) << "make controller port: " << array_ports.back() << std::endl;
            controller = std::make_unique<Controller>(array_ports.back());
            LOG(INFO) << "is_init: " << controller->isInit() << std::endl;

            if(controller->isInit() && getScalePar()) {
                LOG(INFO) << "set connected" << std::endl;
                controller->setConnected(true);
            } else {
                LOG(INFO) << "not connected" << std::endl;
                array_ports.pop_back();
                controller.reset();
                controller = nullptr;
            }
        }

        if(controller && controller->isInit())
        {
            if(need_getMassa) {
                need_getMassa = false;
                if(!m_getMassa()) {
                    LOG(INFO) << "need reconnected" << std::endl;
                    array_ports.clear();
                    controller.reset();
                    controller = nullptr;
                }
            } else if(need_setZero) {
                need_setZero = false;
            } else if(need_setTare) {
                need_setTare = false;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

    }

    LOG(INFO) << "routine stop" << std::endl;
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

