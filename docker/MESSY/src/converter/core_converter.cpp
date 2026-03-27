#include <converter/core_converter.hpp>


void core_converter::set_attributes()
{
    current_out.set_timestep(1, SIM_RESOLUTION);
    current_out.set_delay(1);
}

void core_converter::initialize() {}

void core_converter::processing()
{
    double ideal_power;
    double eta; // Efficiency
    double i_tmp;
    double v_tmp;

    // Read input quantities
    i_tmp = current_in.read();
    v_tmp = voltage_in.read();
    
    // Get efficiency
    eta = lut_eta.get_val(i_tmp) / 100;
    out1.write(in.read());
    //std::cout << "converter" << i_tmp * v_tmp / (VREF_BUS*eta)<< std::endl;
    current_out.write(i_tmp * v_tmp / (VREF_BUS*eta)+in.read());
}
