#include <config.hpp>
#include <core.hpp>
#include <systemc.h>

SC_MODULE(Sensor_mic_click_functional)
{
    Core* core;
    //Input Port
    sc_core::sc_in <bool> enable;
    sc_core::sc_in <unsigned int>   address;
    sc_core::sc_in <uint8_t*>   data_in;
    sc_core::sc_in <unsigned int>   req_size;
    sc_core::sc_in <bool> flag_wr;
    sc_core::sc_in <bool> ready;
    //Output Port
    sc_core::sc_out <uint8_t*>  data_out;
    sc_core::sc_out <bool> go;
    //Power Port
    sc_core::sc_out <int> power_signal;
    //Thermal Port
    //sc_core::sc_out <int> thermal_signal;
    
    SC_CTOR(Sensor_mic_click_functional): 
    enable("Enable_signal"),
    address("Address"),
    data_in("Data_in"),
    flag_wr("Flag"),
    ready("Ready"),
    data_out("Data_out"),
    go("Go"),
    power_signal("Func_to_Power_signal")
    {
        //printf("SENSOR :: systemc constructor");
        register_memory = new uint8_t[256];
        SC_THREAD(sensor_logic);
        sensitive << ready;
    }

    void sensor_logic();

    Sensor_mic_click_functional(){
        //printf("SENSOR :: constructor");
    }

    ~Sensor_mic_click_functional(){
        delete[]register_memory;
    }
    //Register Map
    private: 
    uint8_t* register_memory;
    int register_memory_size=256;
};