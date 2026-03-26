#include <systemc.h>
#include <systemc-ams.h>
#include <config.hpp>
#define mic_click_VREF 3.3


SCA_TDF_MODULE(Sensor_mic_click_power)
{
    //Data from Functional Instance
    sca_tdf::sc_in  <int> func_signal;
    //Data to Power Bus
    sca_tdf::sca_out <double> voltage_state;
    sca_tdf::sca_out <double> current_state;

    SCA_CTOR(Sensor_mic_click_power):
        func_signal("State_of_Power_From_Functional"),
        voltage_state("Voltage_trace_to_Power_Bus"),
        current_state("Current_trace_to_Power_Bus")
    {}

    void set_attributes();
    void initialize();
    void processing();

    Sensor_mic_click_power(){}
};