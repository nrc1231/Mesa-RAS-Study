#include <sensor_mic_click_functional.hpp>

void Sensor_mic_click_functional::sensor_logic(){
    while (true)
    {
        if( enable.read() == true ){
            if(ready.read() == true){
                if( flag_wr.read() == true ){
                    unsigned int add=address.read();
                    // read operation
                    data_out.write(register_memory+add);
                    // reading
                    power_signal.write(mic_click_read);
                    double start_time=sc_time_stamp().to_double();
                    core->request_delay(start_time,30,SIM_RESOLUTION);
                    // idling
                    power_signal.write(mic_click_idle);
                    go.write(true);
                } else {
                    unsigned int add=address.read();
                    // write operation
                    unsigned int req_size_val=req_size.read();
                    uint8_t* req_core_val_addr=data_in.read();
                    for(unsigned int i=0;i<req_size_val;i++) register_memory[i+add] = req_core_val_addr[i];
                    data_out.write(register_memory+add);
                    // writing
                    power_signal.write(mic_click_write);
                    double start_time=sc_time_stamp().to_double();
                    core->request_delay(start_time,30,SIM_RESOLUTION);
                    // idling
                    power_signal.write(mic_click_idle);
                    go.write(true);
                }
            } else {
                go.write(false);
            }

        } else {
            power_signal.write(0);
        }
        wait();
    }   
}

