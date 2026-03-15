#include <harvester_battery.hpp>
void Harvester_battery_battery_voc::set_attributes()
{
    v_oc.set_timestep(1, SIM_RESOLUTION);
    r_s.set_timestep(1, SIM_RESOLUTION);
    soc.set_timestep(1, SIM_RESOLUTION);
    r_s.set_delay(1);
    soc.set_delay(1);
}

void Harvester_battery_battery_voc::initialize() {
    tmpsoc = 1.0f;
    double pre_t=0; // 记录初始时间
    //previous_t = std::chrono::high_resolution_clock::now();
}

void Harvester_battery_battery_voc::processing(){
    double tmpcurrent = i.read(); // Battery current, if negative, the battery is charged 
    //double delta_time=dt.read();
    /* 
    Compute actual state-of-charge solving the integral:
    SOC_t = SOC_{t-1} - \int^{t}_{-inf} i(\tau) / C d\tau
    */
    // TODO: Quickfix for scaling resulution
    

     auto current_time = std::chrono::high_resolution_clock::now();
     auto current_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(current_time.time_since_epoch()).count();
     //std::cout << "[Battery] current_time_step: " << current_time_ms-prev_t<< " ms" << std::endl;
     double dt=current_time_ms-prev_t;
     //std::cout << "[Battery] current_time_step: " << dt<< " ms" << std::endl;
     prev_t=current_time_ms;
     if(dt>=100){
       dt=0;
     }



  
    int scaling_factor_resulution;
    if (SIM_RESOLUTION == 4)
    {
        // MS
        scaling_factor_resulution = 1000;
    }
    else
    {
        // SEC
        scaling_factor_resulution = 1;
    }
    tmpsoc -= (((tmpcurrent + prev_i_batt) * dt) / ( (3600) * 3 * scaling_factor_resulution));
    prev_i_batt = tmpcurrent; // Update
    //cout << "I ="<< tmpcurrent + prev_i_batt<< endl;
    //cout << "SOC ="<< tmpsoc<< endl;
    std::cout << "  "<< std::endl;
    if (tmpsoc >= 1) // Not let the SOC overflow
{
    soc.write(1);
    tmpsoc = 1;
}
else    soc.write(tmpsoc);

if (tmpsoc >= 0 && tmpsoc < 0.025)
{
    vocv = 2.5 * 6;
    rs = 0.0185;
}
if (tmpsoc >= 0.025 && tmpsoc < 0.075)
{
    vocv = 2.9535 * 6;
    rs = 0.0185;
}
if (tmpsoc >= 0.075 && tmpsoc < 0.125)
{
    vocv = 3.1020 * 6;
    rs = 0.0167;
}
if (tmpsoc >= 0.125 && tmpsoc < 0.175)
{
    vocv = 3.2285 * 6;
    rs = 0.0165;
}
if (tmpsoc >= 0.175 && tmpsoc < 0.225)
{
    vocv = 3.3359 * 6;
    rs = 0.0182;
}
if (tmpsoc >= 0.225 && tmpsoc < 0.275)
{
    vocv = 3.3982 * 6;
    rs = 0.0180;
}
if (tmpsoc >= 0.275 && tmpsoc < 0.325)
{
    vocv = 3.4507 * 6;
    rs = 0.0187;
}
if (tmpsoc >= 0.325 && tmpsoc < 0.375)
{
    vocv = 3.4946 * 6;
    rs = 0.0188;
}
if (tmpsoc >= 0.375 && tmpsoc < 0.425)
{
    vocv = 3.5375 * 6;
    rs = 0.0191;
}
if (tmpsoc >= 0.425 && tmpsoc < 0.475)
{
    vocv = 3.5937 * 6;
    rs = 0.0203;
}
if (tmpsoc >= 0.475 && tmpsoc < 0.525)
{
    vocv = 3.6486 * 6;
    rs = 0.0214;
}
if (tmpsoc >= 0.525 && tmpsoc < 0.575)
{
    vocv = 3.7004 * 6;
    rs = 0.0224;
}
if (tmpsoc >= 0.575 && tmpsoc < 0.625)
{
    vocv = 3.7462 * 6;
    rs = 0.0228;
}
if (tmpsoc >= 0.625 && tmpsoc < 0.675)
{
    vocv = 3.7916 * 6;
    rs = 0.0234;
}
if (tmpsoc >= 0.675 && tmpsoc < 0.725)
{
    vocv = 3.8325 * 6;
    rs = 0.0236;
}
if (tmpsoc >= 0.725 && tmpsoc < 0.775)
{
    vocv = 3.8858 * 6;
    rs = 0.0246;
}
if (tmpsoc >= 0.775 && tmpsoc < 0.825)
{
    vocv = 3.9362 * 6;
    rs = 0.0257;
}
if (tmpsoc >= 0.825 && tmpsoc < 0.875)
{
    vocv = 3.9724 * 6;
    rs = 0.0256;
}
if (tmpsoc >= 0.875 && tmpsoc < 0.925)
{
    vocv = 3.9823 * 6;
    rs = 0.0243;
}
if (tmpsoc >= 0.925 && tmpsoc < 0.975)
{
    vocv = 4.0112 * 6;
    rs = 0.0239;
}
if (tmpsoc >= 0.975 && tmpsoc <= 1)
{
    vocv = 4.2 * 6;
    rs = 0.0239;
    //cout << "here! ="<< vocv<< endl;
}
    // Output the battery SOC
    v_oc.write(vocv);
    r_s.write(rs);
    // SOC and battery Voc relationship
    // val(x) = p1*x^4 + p2*x^3 + p3*x^2 + p4*x + p5
    //double v_p1=-13.26,v_p2=27.4,v_p3=-17.99,v_p4=4.496,v_p5=3.45;
    //v_oc.write((v_p1 * tmpsoc * tmpsoc * tmpsoc * tmpsoc) + (v_p2 * tmpsoc * tmpsoc * tmpsoc) + (v_p3 * tmpsoc * tmpsoc) + (v_p4 * tmpsoc) + v_p5); // Place interpolated funct here
    
    // SOC and battery internal resistance relationship
    // val(x) = p1*x^4 + p2*x^3 + p3*x^2 + p4*x + p5
    //double r_p1=-0.1055,r_p2=0.1968,r_p3=-0.1119,r_p4=0.01607,r_p5=0.003117;
    //r_s.write((r_p1 * tmpsoc * tmpsoc * tmpsoc * tmpsoc) + (r_p2 * tmpsoc * tmpsoc * tmpsoc) + (r_p3 * tmpsoc * tmpsoc) + (r_p4 * tmpsoc) + r_p5); // Place interpolated funct here
    
    // When the battery SOC decreases under 5% of starting voc, the simulation stops.	
    if(tmpsoc <= 0.0)
    {
        cout << "SOC is less than or equal to {{starting_voc-perc_voc}}/100%:" << " @" << sc_time_stamp() << " ; it is "<< tmpsoc << endl;
        sc_stop();
    }
}
