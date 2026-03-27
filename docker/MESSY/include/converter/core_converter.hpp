#include <systemc-ams.h>
#include <config.hpp>
#include <lut.hpp>
#define SIZE_CONV_DCDC_core 49
static const double current_CONV_core[SIZE_CONV_DCDC_core] = {0.001, 0.002, 0.003, 0.004, 0.005, 0.006, 0.007, 0.008, 0.009, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 200, 300, 400};
static const double ETA_CONV_core[SIZE_CONV_DCDC_core] = {42.7039, 50.2146, 64.1631, 64.8069, 71.6738, 73.8197, 74.4635, 75.3219, 77.0386, 77.897, 82.4034, 84.1202, 84.7639, 85.4077, 86.0515, 86.2661, 86.2661, 86.4807, 86.9099, 87.7682, 87.7682, 87.9828, 87.9828, 88.1974, 87.9828, 88.1974, 88.1974, 88.1974, 88.412, 88.6266, 87.5536, 87.5536, 87.7682, 88.6266, 88.8412, 88.8412, 89.0558, 90.1288, 90.3433, 90.5579, 90.5579, 90.7725, 90.7725, 91.2017, 90.7725, 90.7725, 90.1288, 88.412, 84.9785};

SCA_TDF_MODULE(core_converter)
{
    sca_tdf::sca_in<double> current_in,in; // Current from PV panel
    sca_tdf::sca_in<double> voltage_in; // Voltage from PV panel
    sca_tdf::sca_out<double> current_out,out1; // Current generated delivered to the BUS

    SCA_CTOR(core_converter): current_in("current_in"),
                            voltage_in("voltage_in"),
                            current_out("current_out") {};

    void set_attributes();
    void initialize();
    void processing();

    core_converter(){}

    private:
        LUT lut_eta = LUT(current_CONV_core, ETA_CONV_core, SIZE_CONV_DCDC_core);
};
