#include <converter/battery_converter.hpp>


void battery_converter::set_attributes()
{
    current_out.set_timestep(1, SIM_RESOLUTION);
    current_out.set_delay(1);
}

void battery_converter::initialize() {}

void battery_converter::processing()
{
    double ideal_power;
    double eta; // Efficiency
    double i_tmp;
    double v_tmp;

    // Read input quantities
    i_tmp = current_in.read();
    v_tmp = voltage_in.read();
    //std::cout << "battery converter "<< i_tmp<< std::endl;
    // Get efficiency
    eta = lut_eta.get_val(i_tmp) / 100;
    //std::cout << "battery converter2 "<<(eta * (i_tmp * VREF_BUS)) / v_tmp+in1.read()<< std::endl;
    current_out.write((eta * (i_tmp * VREF_BUS)) / v_tmp+in1.read());
}
