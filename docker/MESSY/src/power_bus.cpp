#include <power_bus.hpp>

void Power_bus::set_attributes(){}

void Power_bus::initialize(){}

void Power_bus::processing(){
    double tmp_i;
    total_current = 0;

    for (int i = 0; i < NUM_SENSORS; i++){
        tmp_i = current_sensors[i].read();
        total_current += tmp_i;
    }

    tmp_i = core_current.read();
    //std::cout <<"tempi in power bus"<< tmp_i << std::endl;
    total_current += tmp_i;
    //std::cout <<"current"<< total_current/NUM_BATTERIES << std::endl;
    #if NUM_SOURCES>0
    for (int i = 0; i < NUM_SOURCES; i++){
        tmp_i = current_sources[i].read();
        total_current -= tmp_i;
    }
    #endif
    
    #if NUM_BATTERIES>0
    for (int i = 0; i < NUM_BATTERIES; i++)
        current_batteries[i].write(total_current/NUM_BATTERIES);
        out1.write(in1.read());
    #endif
}