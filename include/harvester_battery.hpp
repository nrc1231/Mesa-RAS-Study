#include <systemc-ams.h>
#include <config.hpp>
#include <chrono>


SCA_TDF_MODULE(Harvester_battery_battery_voc)
{
    sca_tdf::sca_in<double> i; // Battery current
    sca_tdf::sca_out<double> v_oc; // Voltage open-circuit
    sca_tdf::sca_out<double> r_s; // Series resistance
    sca_tdf::sca_out<double> soc ; // State of Charge
    double vocv=4.2*6,rs=0.0239;
    double prev_t=0;
    double start_time = 0; // 记录初始时间
    SCA_CTOR(Harvester_battery_battery_voc): v_oc("v_oc"),
                           r_s("r_s"),
                           soc("soc"),
                           tmpsoc((1.0)),
                           prev_i_batt(0) {}

    void set_attributes();
    void initialize();
    void processing();
    
    private:
	    int c_nom=3;
        double tmpsoc=1.0f;
        double prev_i_batt;
};



SC_MODULE(Harvester_battery_battery_char)
{
    // Interface and internal components declaration
    sca_tdf::sca_in<double> r_s, // Internal resistance
                            i, // Battery current
                            v_oc; // Battery V_oc
    sca_tdf::sca_out<double> v; // V_batt

    sca_eln::sca_tdf::sca_isource* I_batt;
    sca_eln::sca_tdf::sca_vsink* V_batt;

    sca_eln::sca_tdf_vsource* V_oc;
    sca_eln::sca_node n1, n2;
    sca_eln::sca_node_ref gnd;
    sca_eln::sca_tdf::sca_r* R_s;

    SC_CTOR(Harvester_battery_battery_char)
    {
        // V_oc voltage instantiation
        V_oc = new sca_eln::sca_tdf_vsource("V_oc");
        V_oc->inp(v_oc);
        V_oc->p(n1);
        V_oc->n(gnd);
        // Internal resistance instantiation
        R_s = new sca_eln::sca_tdf::sca_r("R_s");
        R_s->p(n1);
        R_s->n(n2);
        R_s->scale=1.0;
        R_s->inp(r_s);
        //Load current instantiation
        I_batt = new sca_eln::sca_tdf::sca_isource("I_batt");
        I_batt->inp(i);
        I_batt->p(n2);
        I_batt->n(gnd);
        //Output voltage of the battery
        V_batt = new sca_eln::sca_tdf::sca_vsink("V_batt");
        V_batt->p(n2);
        V_batt->n(gnd);
        V_batt->outp(v);
    }
};

SC_MODULE(Harvester_battery)
{
    // Interface and internal components declaration
    sca_tdf::sca_in<double> i; // Battery current
    sca_tdf::sca_out<double> v; // Battery voltage
    sca_tdf::sca_out<double> soc; // Battery SOC

    // Connecting signals
    sca_tdf::sca_signal<double> v_oc, r_s;

    // Instantiation of battery componenets
    Harvester_battery_battery_voc* voc_module;
    Harvester_battery_battery_char* char_module;
    //std::chrono::high_resolution_clock::time_point previous_t; 
    SC_CTOR(Harvester_battery): i("i"),
                      v("v"),
                      soc("soc")
    {
        voc_module = new Harvester_battery_battery_voc("voc");
        char_module = new Harvester_battery_battery_char("batt");

        voc_module->i(i);
        voc_module->v_oc(v_oc);
        voc_module->r_s(r_s);
        voc_module->soc(soc);

        char_module->r_s(r_s);
        char_module->i(i);
        char_module->v_oc(v_oc);
        char_module->v(v);
    }
};
