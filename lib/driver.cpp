#include "driver.h"

#include <iostream>
#include <memory>

#include "controller.h"
#include "checkcomports.h"

std::unique_ptr<Controller> controller;

Driver::Driver():
    is_run{false}
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
           if(result) {
               setConnected();
           }
       }
    }

    if(!result) {
        LOG(INFO) << "Driver::getScalePar error parse" << std::endl;
    }

    return result;
}

bool Driver::getMassa()
{
    bool result = false;

    if(!(controller && controller->isInit())) {
        std::cout << "Driver::getMassa controller not init" << std::endl;
        return result;
    }

    if(!controller->isConnected()) {
        std::cout << "Driver::getMassa controller not connected" << std::endl;
        return result;
    }

    Data data;
    Data recv_data;
    Protocol::getMassa(data);
    //Protocol::print(data);

    if(controller->send(data)) {
       if(controller->read(recv_data) && Protocol::check_crc(recv_data)) {
           ScalesParameters _params;
           Protocol::parseResponseGetMassa(recv_data, _params);
           setScalesParameters(_params);
           result = true;
       }
    }

    if(!result) {
        resetScaleParameters();
        controller.reset();
        controller = nullptr;
    }

    return result;
}

bool Driver::setZero()
{
    bool result = false;

    if(!(controller && controller->isInit())) {
        std::cout << "Driver::setZero controller not init" << std::endl;
        return result;
    }

    if(!controller->isConnected()) {
        std::cout << "Driver::setZero controller not connected" << std::endl;
        return result;
    }

    Data data;
    Data recv_data;
    Protocol::setZero(data);
    //Protocol::print(data);

    if(controller->send(data)) {
       if(controller->read(recv_data) && Protocol::check_crc(recv_data)) {
           ScalesParameters _params;
           Protocol::parseResponseSetZero(recv_data, _params);
           setScalesParameters(_params);
           result = true;
       }
    }

    if(!result) {
        resetScaleParameters();
        controller.reset();
        controller = nullptr;
    }

    return result;
}

bool Driver::setTare(int32_t tare)
{
    bool result = false;

    if(!(controller && controller->isInit())) {
        std::cout << "Driver::setTare controller not init" << std::endl;
        return result;
    }

    if(!controller->isConnected()) {
        std::cout << "Driver::setTare controller not connected" << std::endl;
        return result;
    }

    Data data;
    Data recv_data;
    Protocol::setTare(data, tare);
    //Protocol::print(data);

    if(controller->send(data)) {
       if(controller->read(recv_data) && Protocol::check_crc(recv_data)) {
           ScalesParameters _params;
           Protocol::parseResponseSetTare(recv_data, _params);
           setScalesParameters(_params);
           result = true;
       }
    }

    if(!result) {
        resetScaleParameters();
        controller.reset();
        controller = nullptr;
    }

    return result;
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

//        if(controller && controller->isInit())
//        {
//            if(need_getMassa) {
//                need_getMassa = false;
//                if(!m_getMassa()) {
//                    LOG(INFO) << "need reconnected" << std::endl;
//                    array_ports.clear();
//                    controller.reset();
//                    controller = nullptr;
//                }
//            } else if(need_setZero) {
//                need_setZero = false;
//            } else if(need_setTare) {
//                need_setTare = false;
//            }
//            std::this_thread::sleep_for(std::chrono::milliseconds(100));
//        } else

        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

    }

    LOG(INFO) << "routine stop" << std::endl;
}

void Driver::resetScaleParameters()
{
    std::lock_guard<std::mutex> _lck(mutexParams);
    scalesParameters.connection     = false;
    scalesParameters.condition      = false;
    scalesParameters.weight         = 0;
    scalesParameters.weight_stable  = false;
    scalesParameters.weight_overmax = false;
    scalesParameters.weight_net     = false;
    scalesParameters.weight_zero    = false;
    (void)_lck;
}

void Driver::setConnected()
{
    std::lock_guard<std::mutex> _lck(mutexParams);
    scalesParameters.connection     = true;
    scalesParameters.condition      = false;
    scalesParameters.weight         = 0;
    scalesParameters.weight_stable  = false;
    scalesParameters.weight_overmax = false;
    scalesParameters.weight_net     = false;
    scalesParameters.weight_zero    = false;
    (void)_lck;
}

void Driver::setScalesParameters(const ScalesParameters& params)
{
    std::lock_guard<std::mutex> _lck(mutexParams);
    scalesParameters = params;
    (void)_lck;
}

void Driver::getScalesParameters(ScalesParameters& get_params)
{
    std::lock_guard<std::mutex> _lck(mutexParams);
    get_params = scalesParameters;
    (void)_lck;
}

void Driver::printScalesParameters()
{
    std::lock_guard<std::mutex> _lck(mutexParams);
    std::cout << "connection: " << scalesParameters.connection << std::endl;
    std::cout << "condition: "  << scalesParameters.condition << std::endl;
    std::cout << std::dec << "weight: "     << (int)scalesParameters.weight << std::endl;

    std::cout << "weight_stable: "  << scalesParameters.weight_stable << std::endl;
    std::cout << "weight_overmax: " << scalesParameters.weight_overmax << std::endl;
    std::cout << "weight_net: "     << scalesParameters.weight_net << std::endl;
    std::cout << "weight_zero: "    << scalesParameters.weight_zero << std::endl;
    (void)_lck;
}

std::string Driver::get_version() const
{
    return "1.0";
}

