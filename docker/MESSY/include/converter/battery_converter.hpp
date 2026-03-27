#include <systemc-ams.h>
#include <config.hpp>
#include <lut.hpp>
#define SIZE_CONV_DCDC_battery 22
static const double current_CONV_battery[SIZE_CONV_DCDC_battery] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 200, 300, 400};
static const double ETA_CONV_battery[SIZE_CONV_DCDC_battery] = {88.0165, 90.9091, 91.1157, 92.1488, 92.7686, 92.7686, 93.595, 93.8017, 92.9752, 93.1818, 93.595, 94.2149, 94.4215, 94.6281, 94.4215, 94.4215, 94.4215, 94.4215, 94.6281, 94.4215, 94.6281, 96.6942};

SCA_TDF_MODULE(battery_converter)
{
    sca_tdf::sca_in<double> current_in,in1; // Current from PV panel
    sca_tdf::sca_in<double> voltage_in; // Voltage from PV panel
    sca_tdf::sca_out<double> current_out; // Current generated delivered to the BUS

    SCA_CTOR(battery_converter): current_in("current_in"),
                            voltage_in("voltage_in"),
                            current_out("current_out") {};

    void set_attributes();
    void initialize();
    void processing();

    battery_converter(){}

    private:
        LUT lut_eta = LUT(current_CONV_battery, ETA_CONV_battery, SIZE_CONV_DCDC_battery);
};
