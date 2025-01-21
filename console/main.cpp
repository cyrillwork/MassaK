#include <iostream>
#include <cstring>
#include <string>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <pthread.h>
#include <errno.h>
#include <thread>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <map>
#include <memory>
#include <dlfcn.h>

static void* dhndl_logic;

#include "driver_plain.h"


using f_plain = void (*)(void);
using f_bool  = bool (*)(void);
using f_tare  = bool (*)(int32_t);
using f_get_scale_params  = void (*)(ScalesParameters*);

static f_plain	g_start  = nullptr;
static f_plain	g_stop   = nullptr;

static f_bool	g_get_scale_par = nullptr;
static f_bool	g_get_massa     = nullptr;
static f_bool	g_set_zero      = nullptr;
static f_tare	g_set_tare      = nullptr;
static f_get_scale_params	g_get_scale_params      = nullptr;

static std::unique_ptr<std::thread> gameThread = nullptr;

bool MyLoadLibrary(const std::string &libPath){
    bool result = true;

    dhndl_logic = dlopen(libPath.c_str(), RTLD_NOW);

    if (dhndl_logic == NULL) {
        fputs (dlerror(), stderr);
        printf("cannot open %s\n", libPath.c_str());
        return !result;
    }

    g_start = reinterpret_cast<f_plain>(dlsym(dhndl_logic, "start"));
    if(!g_start) {
        std::cout << "Error load g_start " << dlerror() << std::endl;
        result = false;
    }

    g_stop = reinterpret_cast<f_plain>(dlsym(dhndl_logic, "stop"));
    if(!g_stop) {
        std::cout << "Error load g_stop " << dlerror() << std::endl;
        result = false;
    }

    g_get_scale_par = reinterpret_cast<f_bool>(dlsym(dhndl_logic, "getScalePar"));
    if(!g_stop) {
        std::cout << "Error load g_get_scale_par " << dlerror() << std::endl;
        result = false;
    }

    g_get_massa = reinterpret_cast<f_bool>(dlsym(dhndl_logic, "getMassa"));
    if(!g_stop) {
        std::cout << "Error load g_get_massa " << dlerror() << std::endl;
        result = false;
    }

    g_set_zero = reinterpret_cast<f_bool>(dlsym(dhndl_logic, "setZero"));
    if(!g_stop) {
        std::cout << "Error load g_set_zero " << dlerror() << std::endl;
        result = false;
    }

    g_set_tare = reinterpret_cast<f_tare>(dlsym(dhndl_logic, "setTare"));
    if(!g_stop) {
        std::cout << "Error load g_set_tare " << dlerror() << std::endl;
        result = false;
    }

    g_get_scale_params = reinterpret_cast<f_get_scale_params>(dlsym(dhndl_logic, "getScalesParameters"));
    if(!g_stop) {
        std::cout << "Error load g_get_scale_params " << dlerror() << std::endl;
        result = false;
    }

    return result;
}

int main(int arg, char* argc[])
{
    std::string libPath = "../lib/libMassaK.so.1";
    std::cout << "Test MassaK, press key" << std::endl;
    std::cout << "q quit, m get massa, z set zero, t number - set tare" << std::endl;

    if(!MyLoadLibrary(libPath)) {
        std::cout << "Error with load lib " << libPath << std::endl;
        return 1;
    }

    g_start();

    std::thread t( []() {
        while(true)
        {
            ScalesParameters params;
            std::string input;

            std::getline(std::cin, input);

            if(input == "q" || input == "quit") {
                break;
            }

            if(input == "m") {
                auto res = g_get_massa();
                std::cout << "getMassa res: " << res << std::endl;
            }
            if(input == "z") {
                auto res = g_set_zero();
                std::cout << "setZero res: " << res << std::endl;
            }
            if((input.size() > 0) && (input[0] == 't')) {
                int value = 0;
                ::sscanf(input.data(), "t %d", &value);
                std::cout << "value: " << value << std::endl;
                auto res = g_set_tare(value);
                std::cout << "setZero res: " << res << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));

            g_get_scale_params(&params);


            std::string str_info;
            str_info =  "connection:\t"       + std::string(params.connection ? "true" : "false") + "\n" +
                        "condition:\t"        + std::string(params.condition ? "true" : "false")      + "\n" +
                        "weigth:\t\t"         + std::to_string(params.weight)         + "\n" +
                        "weight_stable:\t"    + std::string(params.weight_stable ? "true" : "false")  + "\n" +
                        "weight_overmax:\t"   + std::string(params.weight_overmax ? "true" : "false") + "\n" +
                        "weight_net:\t"       + std::string(params.weight_net ? "true" : "false")     + "\n" +
                        "weight_zero:\t"      + std::string(params.weight_zero ? "true" : "false");

            std::cout << str_info << std::endl;
        }

    });
    t.join();

    g_stop();

    if (dhndl_logic) {
        dlclose(dhndl_logic);
    }

    std::cout << "Test MassaK end" << std::endl;
    return 0;
}
