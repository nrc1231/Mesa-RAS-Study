#include <sensor_mic_click_power.hpp>

void Sensor_mic_click_power::set_attributes()
{
    func_signal.set_rate(1);
    func_signal.set_timestep(1, SIM_RESOLUTION);
}

void Sensor_mic_click_power::initialize() {}

void Sensor_mic_click_power::processing()
{
    if(func_signal.read() == mic_click_read ){
        voltage_state.write(mic_click_VREF);
        current_state.write(0.0);
        return;
    }
    if(func_signal.read() == mic_click_write ){
        voltage_state.write(mic_click_VREF);
        current_state.write(0.0);
        return;
    }
    if(func_signal.read() == mic_click_idle ){
        voltage_state.write(mic_click_VREF);
        current_state.write(0.0);
        return;
    }
    if(func_signal.read() == 0){
        voltage_state.write(0.0);
        current_state.write(0.0);
        return;
    }
}