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

#ifdef MASSAK_WINDOWS
#include <windows.h>
static HINSTANCE dhndl_logic = nullptr;
#else
#include <dlfcn.h>
static void* dhndl_logic;
#endif


#include "driver_plain.h"


using f_plain = void (*)(void);
using f_bool  = bool (*)(void);
using f_tare  = bool (*)(int32_t);
using f_get_scale_params  = void (*)(ScalesParameters*);

static f_bool	g_get_scale_par = nullptr;
static f_bool	g_set_zero      = nullptr;
static f_tare	g_set_tare      = nullptr;
static f_get_scale_params	g_get_scale_params      = nullptr;

static std::unique_ptr<std::thread> gameThread = nullptr;

bool MyLoadLibrary(const std::string &libPath){

#ifdef MASSAK_WINDOWS
    std::wstring str1(libPath.begin(), libPath.end());
    dhndl_logic = LoadLibrary(str1.c_str());

    if(!dhndl_logic) {
        //fputs (dlerror(), stderr);
        printf("cannot open %s\n", libPath.c_str());
        return !result;
    }

    g_get_scale_par = reinterpret_cast<f_bool>(GetProcAddress(dhndl_logic, "GetScalesParameters"));
    if(!g_get_scale_par) {
        std::cout << "Error load g_get_scale_par " << std::endl;
        result = false;
    }

    g_set_zero = reinterpret_cast<f_bool>(GetProcAddress(dhndl_logic, "SetZero"));
    if(!g_get_scale_par) {
        std::cout << "Error load g_set_zero " << std::endl;
        result = false;
    }

    g_set_tare = reinterpret_cast<f_tare>(GetProcAddress(dhndl_logic, "SetTare"));
    if(!g_set_tare) {
        std::cout << "Error load g_set_tare " << std::endl;
        result = false;
    }

    g_get_scale_params = reinterpret_cast<f_get_scale_params>(GetProcAddress(dhndl_logic, "getScalesParametersStruct"));
    if(!g_set_tare) {
        std::cout << "Error load g_get_scale_params " << std::endl;
        result = false;
    }

#else
    bool result = true;

    dhndl_logic = dlopen(libPath.c_str(), RTLD_NOW);

    if (dhndl_logic == NULL) {
        fputs (dlerror(), stderr);
        printf("cannot open %s\n", libPath.c_str());
        return !result;
    }

    g_get_scale_par = reinterpret_cast<f_bool>(dlsym(dhndl_logic, "GetScalesParameters"));
    if(!g_get_scale_par) {
        std::cout << "Error load g_get_scale_par " << dlerror() << std::endl;
        result = false;
    }


    g_set_zero = reinterpret_cast<f_bool>(dlsym(dhndl_logic, "SetZero"));
    if(!g_set_zero) {
        std::cout << "Error load g_set_zero " << dlerror() << std::endl;
        result = false;
    }

    g_set_tare = reinterpret_cast<f_tare>(dlsym(dhndl_logic, "SetTare"));
    if(!g_set_tare) {
        std::cout << "Error load g_set_tare " << dlerror() << std::endl;
        result = false;
    }

    g_get_scale_params = reinterpret_cast<f_get_scale_params>(dlsym(dhndl_logic, "getScalesParametersStruct"));
    if(!g_get_scale_params) {
        std::cout << "Error load g_get_scale_params " << dlerror() << std::endl;
        result = false;
    }

#endif

    return result;
}

int main(int arg, char* argc[])
{
#ifdef MASSAK_WINDOWS
    std::string libPath = "../lib/MassaK.dll";
#else
    std::string libPath = "../lib/libMassaK.so.1";
#endif


    std::cout << "Test MassaK, press key" << std::endl;
    std::cout << "q quit, p - get weight, z set zero, t number - set tare" << std::endl;

    if(!MyLoadLibrary(libPath)) {
        std::cout << "Error with load lib " << libPath << std::endl;
        return 1;
    }


    std::thread t( []() {
        while(true)
        {
            ScalesParameters params;
            std::string input;

            std::getline(std::cin, input);

            if(input == "q" || input == "quit") {
                break;
            }

            if(input == "p") {
                auto res = g_get_scale_par();
                std::cout << "g_get_scale_par res: " << res << std::endl;
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

#ifdef MASSAK_WINDOWS
    if(dhndl_logic) {
        FreeLibrary(dhndl_logic);
    }
#else
    if (dhndl_logic) {
        dlclose(dhndl_logic);
    }
#endif


    std::cout << "Test MassaK end" << std::endl;
    return 0;
}
