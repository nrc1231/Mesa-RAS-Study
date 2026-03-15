#include <config.hpp>
#include <functional_bus.hpp>

void Functional_bus::processing_data(){
    unsigned int add_s;
    uint8_t* data_s;
    unsigned int reg_s;
    unsigned int size_s;
    bool flag_s;
    
    while (true){
        if (request_ready.read() == true) {
            add_s = request_address.read();
            flag_s = flag_from_core.read();
            size_s = request_size.read();
            
            if
            (mic_click_BASE_ADDRESS <= add_s && add_s < (mic_click_BASE_ADDRESS + 256)) 
            {
                if(add_s+size_s > (mic_click_BASE_ADDRESS + 256)){
                    fprintf(stderr,"\n[MESSY]\tRead/Write at %d of %d bytes exceeds mic_click memory\n",add_s,size_s);
                    selected_sensor=-1;
                    request_value.write(0x0);
                    request_go.write(true);
                }
                else{
                    reg_s = add_s - mic_click_BASE_ADDRESS;
                    data_s = request_data.read();
                    size_out_sensor[0].write(size_s);
                    address_out_sensor[0].write(reg_s);
                    data_out_sensor[0].write(data_s);
                    flag_out_sensor[0].write(flag_s);
                    selected_sensor = 0;
                    ready_sensor[selected_sensor].write(true);
                }
            }
            else{
                fprintf(stderr,"\n[MESSY]\tAddress %d not found by functional bus\n", add_s);
                selected_sensor=-1;
                request_value.write(0x0);
                request_go.write(true);
            }
            idx_sensor.write(selected_sensor);
        }
        
        wait();

        if(selected_sensor>=0){

            response();

            while (go_sensors[selected_sensor].read() != false && request_ready.read() != false) {
                wait();
                //std::cout << "WAIT!" << std::endl;
            }
            
            request_go.write(false);
        }
 
    }
}    

void Functional_bus::response(){
    if (go_sensors[selected_sensor].read() == true) {
        request_value.write(data_input_sensor[selected_sensor].read());
        request_go.write(true);
        ready_sensor[selected_sensor].write(false);
    }
}