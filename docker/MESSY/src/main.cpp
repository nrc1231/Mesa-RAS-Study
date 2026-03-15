#include <systemc.h>
#include <systemc-ams.h>
#include <core.hpp>
#include <core_power.hpp>
#include <converter/core_converter.hpp>
#include <functional_bus.hpp>
#include <power_bus.hpp>
#include <converter/load_converter.hpp>

#include <sensor_mic_click_functional.hpp>
#include <sensor_mic_click_power.hpp>
#include <chrono>
#include <harvester_battery.hpp>
#include <converter/battery_converter.hpp>
#include <systemc-ams.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <systemc-ams.h>
#include <iostream>
#include <array>
#include <iomanip>
#include <fcntl.h>




#include <systemc-ams>
#include <vector>
#include <cmath>
#include <complex>
#include <random>
#include <systemc-ams.h>
double cs=0, vy = 0, sc = 0,z=0,ab=0,i=0,xw=0,vw=0;

#include <fcntl.h>

#include <fcntl.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <cstring>
#include <iostream>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <cstring>
#include <iostream>
double water_level =0;
double target_level =0;
double vs = 0, vrel = 0,ec=150,fdes=0,gamma1=0,r=12.3,pa=0,ps=0,cs2=0,fdes2=0,fdes3=0,fdes4=0,cs3=0,cs4=0,arl=0,arr=0,vrel2=0,vrel3=0,vrel4=0,vrel1=0,CV=0;

// 时间戳（Europe/Rome）：2026-01-14 13:00

// 时间戳（Europe/Rome）：2026-01-14 13:xx

SCA_TDF_MODULE(input_generator)
{
    sca_tdf::sca_out<double> out;    // 输出控制力给 SystemC 其他模块
    sca_tdf::sca_out<double> out1;   // 可选

    int sock = -1;
    bool connected = false;

    // 四个从 Modelica/LF 收到的量
    

    // 控制力（保持你原来的名字和用法）
  

    // ---------------------------
    // 读取一行字符串 ("x1,x2,x3,x4\n")
    // ---------------------------
    std::string recvLine() {
        std::string s;
        char c;
        while (true) {
            int r = recv(sock, &c, 1, 0);
            if (r <= 0) return "";   // fail or no data
            if (c == '\n') break;
            s.push_back(c);
        }
        return s;
    }

    // ---------------------------
    // 原来的 parseTwo 现在解析四个数 "a,b,c,d"
    // 名字不变，内部实现改为“四拆”
    // ---------------------------
 bool parseEight(const std::string& s,
                double& a, double& b, double& c, double& d,
                double& e, double& f, double& g, double& h)
{
    try {
        size_t start = 0;
        size_t pos   = s.find(',', start);
        if (pos == std::string::npos) return false;
        a = std::stod(s.substr(start, pos - start));

        start = pos + 1;
        pos   = s.find(',', start);
        if (pos == std::string::npos) return false;
        b = std::stod(s.substr(start, pos - start));

        start = pos + 1;
        pos   = s.find(',', start);
        if (pos == std::string::npos) return false;
        c = std::stod(s.substr(start, pos - start));

        start = pos + 1;
        pos   = s.find(',', start);
        if (pos == std::string::npos) return false;
        d = std::stod(s.substr(start, pos - start));

        start = pos + 1;
        pos   = s.find(',', start);
        if (pos == std::string::npos) return false;
        e = std::stod(s.substr(start, pos - start));

        start = pos + 1;
        pos   = s.find(',', start);
        if (pos == std::string::npos) return false;
        f = std::stod(s.substr(start, pos - start));

        start = pos + 1;
        pos   = s.find(',', start);
        if (pos == std::string::npos) return false;
        g = std::stod(s.substr(start, pos - start));

        start = pos + 1;
        // 第八个直接到行尾
        h = std::stod(s.substr(start));
    } catch (...) {
        return false;
    }
    return true;
}


    // ---------------------------
    // 发送 double（回给 LF）
    // 现在改为发送 4 个用逗号隔开的数："cs,0,0,0\n"
    // 以后你有 4 个真正的输出量时，直接替换这 4 个变量即可
    // ---------------------------
    bool sendDouble(double v) {
        // 当前只有一个 cs，就先占位发 4 个值：cs,0,0,0
        double v1 = v;
        double v2 = 0.0;
        double v3 = 0.0;
        double v4 = 0.0;

        std::string s =
            std::to_string(cs) + "," +
            std::to_string(cs2) + "," +
            std::to_string(cs3) + "," +
            std::to_string(cs4) +"," +
            std::to_string(CV) +"\n";

        const char* p = s.c_str();
        int len  = static_cast<int>(s.size());
        int sent = 0;
        while (sent < len) {
            int r = send(sock, p + sent, len - sent, 0);
            if (r <= 0) return false;
            sent += r;
        }
        return true;
    }

    // ---------------------------
    void set_attributes() {
        set_timestep(1, SC_MS);
    }

    // ---------------------------
    void initialize()
    {
        std::cout << "[SC] initialize()\n";

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            std::cout << "[SC] socket() failed\n";
            return;
        }

        // 非阻塞
        fcntl(sock, F_SETFL, O_NONBLOCK);

        struct hostent* h = gethostbyname("host.docker.internal");
        if (!h) {
            std::cout << "[SC] DNS failed\n";
            return;
        }

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port   = htons(3000);
        memcpy(&addr.sin_addr, h->h_addr_list[0], h->h_length);

        int ret = connect(sock, (struct sockaddr*)&addr, sizeof(addr));

        if (ret == 0) {
            connected = true;
            std::cout << "[SC] Connected immediately!\n";
        }
        else if (errno == EINPROGRESS) {
            std::cout << "[SC] Connect pending...\n";
        }
        else {
            std::cout << "[SC] Connect error\n";
        }
    }

    // ---------------------------
    void processing()
    {
        // 检查连接是否完成
        if (!connected) {
            int err = 0;
            socklen_t len = sizeof(err);
            getsockopt(sock, SOL_SOCKET, SO_ERROR, &err, &len);
            if (err == 0) {
                connected = true;
                std::cout << "[SC] TCP fully connected!\n";
            } else return;
        }

        // =====================================================
        // (1) 从 LF 接收 "afl,afr,arl,arr\n"
        // =====================================================
        std::string line = recvLine();

        if (!line.empty()) {
            // 这里用 afl, afr, arl, arr 四个变量来接受
            if (parseEight(line, vs, vrel, arl, arr, vrel1, vrel2, vrel3, vrel4)) {
                std::cout << "[SC] From MDL: "
                          << "afl=" << vs << ", afr=" << vrel
                          << ", arl=" << arl << ", arr=" << arr<<",vrel1="<<vrel1<<",vrel2="<<vrel2<<",vrel3="<<vrel3<<",vrel4="<<vrel4 << "\n";
            } else {
                std::cout << "[SC] Bad message: " << line << "\n";
                return;
            }
        } else {
            // 无消息，先不退出
            return;
        }

        // =====================================================
        // (2) 在此处进行你的主动悬挂控制器计算
        //     现在仍然是占位，后面你可以用 afl/afr/arl/arr 来算 cs
        // =====================================================
        // 例子：先随便给个假逻辑，后面你换成自己的控制器
        // cs = -5.0 * afr;
        // cs = -(afl + afr + arl + arr) * 100.0;

        // =====================================================
        // (3) 把控制力回传给 LF（再转发到 Modelica）
        //     现在按字符串 "cs,0,0,0\n" 发送
        // =====================================================
        if (!sendDouble(cs)) {
            std::cout << "[SC] send control failed\n";
        }
        std::cout << "cs = " << cs << std::endl;

        // =====================================================
        // (4) 输出到 SystemC-AMS 其他模块
        // =====================================================
        out.write(cs);
        out1.write(cs);
    }

    SCA_CTOR(input_generator) {}
};




SCA_TDF_MODULE(active_suspension_module)
{
    sca_tdf::sca_in<double> r_in;    // road input
    sca_tdf::sca_in<double> fs_in;   // active force input

    sca_tdf::sca_out<double> xb_out; // body position
    sca_tdf::sca_out<double> sd_out; // suspension deflection
    sca_tdf::sca_out<double> ab_out; // body acceleration
    sca_tdf::sca_out<double> xw_out; // wheel position
    //sca_tdf::sca_out<double> vw_out;
    //sca_tdf::sca_out<double> xd_out;
    active_suspension_module(sc_core::sc_module_name nm)
        : r_in("r_in"), fs_in("fs_in"),
        xb_out("xb_out"), sd_out("sd_out"),
        ab_out("ab_out"), xw_out("xw_out"),
        mb(300.0), mw(60.0), ks(16000.0), bs(1000.0), kt(190000.0),
        dt(0.001) {
    }

   
     void processing()
    {
      
    double gamma=fdes/(-1250*vrel1);
    double gamma2=fdes2/(-1250*vrel2);
    double gamma3=fdes3/(-1250*vrel3);
    double gamma4=fdes4/(-1250*vrel4);
      
    double phi=124;
    double rm=12.3;
    static double ssig=1,ssig2=1,ssig3=1,ssig4=1;
    if(vrel1>=0){
      ssig=1;
    }
    if(vrel1<0){
      ssig=-1;
    }

    if(vrel2>=0){
      ssig2=1;
    }
    if(vrel2<0){
      ssig2=-1;
    }

    if(vrel3>=0){
      ssig3=1;
    }
    if(vrel3<0){
      ssig3=-1;
    }

    if(vrel4>=0){
      ssig4=1;
    }
    if(vrel4<0){
      ssig4=-1;
    }
      
    static double var=0,var2=0,var3=0,var4=0;
    double Ic=0,Ic2=0,Ic3=0,Ic4=0,Itot=0;
    if(gamma<=0){
      var=fabs((phi*(ssig*ec-phi*vrel1))/fdes)-rm;
      if(var<0){
        var=0;
      }
      Ic=(ssig*ec-phi*vrel1)/(var+rm);         
      if(fabs(fdes)<1||fabs(vrel1)<0.01){
        Ic=0;
      }
      cs=-phi*Ic;
    }
    if(gamma>0&&gamma<1){
      var=fabs((phi*(ssig*ec-phi*vrel1))/fdes)-rm;
       
      Ic=(ssig*ec-phi*vrel1)/(var+rm);
      
      if(fabs(fdes)<1e-2){
        Ic=0;
      }
      if(var<0){
        var=fabs((-phi*phi*vrel1)/fdes)-rm;
        Ic=0;
        cs=(-phi*phi*vrel1)/(rm+var);
        
      }
      if(var>=0){
        cs=phi*Ic;
      }
     
      
    }
    if(gamma>=1){
      var=fabs((phi*(-ssig*ec-phi*vrel1))/fdes)-rm;
      if(var<0){
        var=0;
      }
      if(fabs(fdes)<1e-2){
        Ic=0;
      }
      Ic=(-ssig*ec-phi*vrel1)/(var+rm);    
      cs=phi*Ic;     //here
    }

    if(gamma2<=0){
      var2=fabs((phi*(ssig2*ec-phi*vrel2))/fdes2)-rm;
      if(var2<0){
        var2=0;
      }
      Ic2=(ssig2*ec-phi*vrel2)/(var2+rm);         
      if(fabs(fdes2)<1||fabs(vrel2)<0.01){
        Ic2=0;
      }
      cs2=-phi*Ic2;
    }
    if(gamma2>0&&gamma2<1){
      var2=fabs((phi*(ssig2*ec-phi*vrel2))/fdes2)-rm;
       
      Ic2=(ssig2*ec-phi*vrel2)/(var2+rm);
      
      if(fabs(fdes2)<1e-2){
        Ic2=0;
      }
      if(var2<0){
        var2=fabs((-phi*phi*vrel2)/fdes2)-rm;
        Ic2=0;
        cs2=(-phi*phi*vrel2)/(rm+var2);
        
      }
      if(var2>=0){
        cs2=phi*Ic2;
      }
     
      
    }
    if(gamma2>=1){
      var2=fabs((phi*(-ssig2*ec-phi*vrel2))/fdes2)-rm;
      if(var2<0){
        var2=0;
      }
      if(fabs(fdes2)<1e-2){
        Ic2=0;
      }
      Ic2=(-ssig2*ec-phi*vrel2)/(var2+rm);    
      cs2=phi*Ic2;
    }


      if(gamma3<=0){
      var3=fabs((phi*(ssig3*ec-phi*vrel3))/fdes3)-rm;
      if(var3<0){
        var3=0;
      }
      Ic3=(ssig3*ec-phi*vrel3)/(var3+rm);         
      if(fabs(fdes3)<1||fabs(vrel3)<0.01){
        Ic3=0;
      }
      cs3=-phi*Ic3;
    }
    if(gamma3>0&&gamma3<1){
      var3=fabs((phi*(ssig3*ec-phi*vrel3))/fdes3)-rm;
       
      Ic3=(ssig3*ec-phi*vrel3)/(var3+rm);
      
      if(fabs(fdes3)<1e-2){
        Ic3=0;
      }
      if(var3<0){
        var3=fabs((-phi*phi*vrel3)/fdes3)-rm;
        Ic3=0;
        cs3=(-phi*phi*vrel3)/(rm+var3);
        
      }
      if(var3>=0){
        cs3=phi*Ic3;
      }
     
      
    }
    if(gamma3>=1){
      var3=fabs((phi*(-ssig3*ec-phi*vrel3))/fdes3)-rm;
      if(var3<0){
        var3=0;
      }
      if(fabs(fdes3)<1e-2){
        Ic3=0;
      }
      Ic3=(-ssig3*ec-phi*vrel3)/(var3+rm);    
      cs3=phi*Ic3;
    }




    
      if(gamma4<=0){
      var4=fabs((phi*(ssig4*ec-phi*vrel4))/fdes4)-rm;
      if(var4<0){
        var4=0;
      }
      Ic4=(ssig4*ec-phi*vrel4)/(var4+rm);         
      if(fabs(fdes4)<1||fabs(vrel4)<0.01){
        Ic4=0;
      }
      cs4=-phi*Ic4;
    }
    if(gamma4>0&&gamma4<1){
      var4=fabs((phi*(ssig4*ec-phi*vrel4))/fdes4)-rm;
       
      Ic4=(ssig4*ec-phi*vrel4)/(var4+rm);
      
      if(fabs(fdes4)<1e-2){
        Ic4=0;
      }
      if(var4<0){
        var4=fabs((-phi*phi*vrel4)/fdes4)-rm;
        Ic4=0;
        cs4=(-phi*phi*vrel4)/(rm+var4);
        
      }
      if(var4>=0){
        cs4=phi*Ic4;
      }
     
      
    }
    if(gamma4>=1){
      var4=fabs((phi*(-ssig4*ec-phi*vrel4))/fdes4)-rm;
      if(var4<0){
        var4=0;
      }
      if(fabs(fdes4)<1e-2){
        Ic4=0;
      }
      Ic4=(-ssig4*ec-phi*vrel4)/(var4+rm);   
      
      cs4=phi*Ic4;
    }
    Itot=Ic+Ic2+Ic3+Ic4;
    cs=-cs;
    cs2=-cs2;
    cs3=-cs3;
    cs4=-cs4;
      
    xb_out.write(Itot);

    xw_out.write(2);
    sd_out.write(ps);
    ab_out.write(4);
}


    void set_attributes() {
        set_timestep(sca_core::sca_time(dt, sc_core::SC_SEC));
    }

private:
    sca_tdf::sca_ss state_space1;

    sca_util::sca_matrix<double> a, b, c, d;
    sca_util::sca_vector<double> s;            //state-vector

    double mb, mw;
    double ks, bs, kt;
    double dt;
};

SCA_TDF_MODULE(plotter)   // Capacitor + motor energy model
{
    sca_tdf::sca_in<double> xb_in;   // sprung velocity or displacement
    sca_tdf::sca_in<double> sd_in;   // relative velocity (actuator speed)
    sca_tdf::sca_in<double> ab_in;   // actuator force OR control info
    sca_tdf::sca_out<double> out1;   // output capacitor voltage ec

    // -------- parameters --------
    double C  = 0.05;   // capacitance [F]
    double phi = 124.0;  // motor constant
    double r_m = 12.3;   // motor internal resistance
    double SOC=0.9;
    double Voc=3.4;
    //double Rvar = 5.0;  // variable resistance (暂时给常数 or 后面接进来)

    // -------- state --------
    //double ec = 5.0;    // capacitor voltage [V]

    void set_attributes() {
        set_timestep(1, SC_MS);
    }

    void processing() {
        //ec-=(xb_in.read()*0.001/C);
        std::cout << "[Capacitor] E = " <<  ec << std::endl;
        //CV=ec;
        // output capacitor voltage
        static double Pdyn=0;
        static double Pev=0;
        double alpha=0.098,beta=10.223,ga=0.989,sigm=0.000174,mass=1800,velo=20;
      
        Pdyn=(alpha+beta*0+ga*0+sigm*velo*velo)*mass*velo;
        Pev=Pdyn+1.86+164.069*velo+0.031*velo*velo+0.000513*Pdyn*Pdyn*0.0001*0.0001;
        double Id=Pev/ec;
        static double Is=0;
        Is=xb_in.read();
        if(Is<0){Is=-Is;}
        double Isum=Id+Is;
        //if(Is<0){Is=-Is;}
        double Ia=(fdes+fdes2+fdes3+fdes4)/124;
        SOC=SOC-(((Isum)*0.001)*1.36)/(161*36*2);//Is*0.001+Id*0.001
        std::cout<<"Is= "<<Is<<"Id="<<Id<<"Isum="<<Isum<<std::endl;
        if(SOC<=1&&SOC>0.96){
          Voc=3.4;
        }
        else if(SOC<=0.96&&SOC>0.935){
          Voc=3.34;
        }
        else if(SOC<=0.935&&SOC>0.925){
          Voc=3.3162;
        }
        else if(SOC<=0.925&&SOC>0.915){
          Voc=3.3161;
        }
        else if(SOC<=0.915&&SOC>0.905){
          Voc=3.316;
        }
        else if(SOC<=0.905&&SOC>0.895){
          Voc=3.309;
        }
        else if(SOC<=0.895&&SOC>0.885){
          Voc=3.308;
        }
        else if(SOC<=0.885&&SOC>0.875){
          Voc=3.301;
        }
        else if(SOC<=0.875&&SOC>0.865){
          Voc=3.3;
        }
        else if(SOC<=0.865&&SOC>0.825){
          Voc=3.298;
        }
        else if(SOC<=0.825&&SOC>0.775){
          Voc=3.297;
        }
        else if(SOC<=0.775&&SOC>0.725){
          Voc=3.294;
        }
        else if(SOC<=0.725&&SOC>0.675){
          Voc=3.293;
        }
        else if(SOC<=0.675&&SOC>0.625){
          Voc=3.283;
        }
        else if(SOC<=0.625&&SOC>0.575){
          Voc=3.27;
        }
        else if(SOC<=0.575&&SOC>0.525){
          Voc=3.269;
        }
        else if(SOC<=0.525&&SOC>0.475){
          Voc=3.26;
        }
        else if(SOC<=0.475&&SOC>0.425){
          Voc=3.242;
        }
        else if(SOC<=0.425&&SOC>0.375){
          Voc=3.241;
        }
        else if(SOC<=0.375&&SOC>0.325){
          Voc=3.224;
        }
        else if(SOC<=0.325&&SOC>0.275){
          Voc=3.185;
        }
        else if(SOC<=0.275&&SOC>0.225){
          Voc=3.178;
        }
        else if(SOC<=0.225&&SOC>0.175){
          Voc=3.147;
        }
        else if(SOC<=0.175&&SOC>0.125){
          Voc=3.023;
        }
        else if(SOC<=0.125&&SOC>0.075){
          Voc=2.882;
        }
        else if(SOC<=0.075&&SOC>0.025){
          Voc=2.726;
        }
        else if(SOC<=0.025){
          Voc=2.545;
        }
        ec=108*Voc-(Is+Id)*108*0.0004;
        CV=SOC; //Is*ec
        out1.write(ec);
    }

    SCA_CTOR(plotter) {}
};


SCA_TDF_MODULE(additional_energy_cost)       //Thrust translator
   {
    //sca_tdf::sca_in<double> in;
    sca_tdf::sca_out<double> out;
    //sca_tdf::sca_de::sca_out<double> outp;
    
    void set_attributes() {
        set_timestep(1, SC_MS);
    }

    void processing() {
       
      out.write(0); 
      //outp.write(delta_time);
    }

    SCA_CTOR(additional_energy_cost) {}
};


SC_MODULE(To_core) {        //This module is used to send the sensor data to the core use the "middle memory"
    //sc_in<double> in;
    sc_out<double> out,out1,out2,out3;
    int i=0;
    SC_CTOR(To_core) {
        SC_THREAD(processing);
    }

    void processing() {
        while (true) {
            out.write(vs);  //spring compress
            out1.write(vrel); //vertical acc 
            out2.write(arl);  //spring compress
            out3.write(arr); //vertical acc 
            
            wait(1, SC_MS); 
        }
    }
};

SC_MODULE(From_core) {        //This module is used to receive the control data from the core use the "middle memory"
    sc_in<double> in,in1,in2,in3;
    //sc_out<double> out,out1;
    int i=0;
    SC_CTOR(From_core) {
        SC_THREAD(processing);
    }

    void processing() {
        while (true) {
            //double a=in.read();
            //cs=in.read()/1000000;
            //cs=0;
            //cs=cs*6;
            //out.write(cs);
            //out.write(in.read());
            //out1.write(in1.read());
            fdes2=in.read()/1000;
            fdes=in1.read()/1000;
            fdes3=in2.read()/1000;
            fdes4=in3.read()/1000;
            //static double R_prev=0;
            //Rvar=0.8*Rvar+0.2*R_prev;
            //R_prev=Rvar;
        
            //std::cout << "Fdes= " << fdes << std::endl;
            //std::cout << "R= " << in1.read() << std::endl;
            wait(1, SC_MS); // 每1毫秒更新一次，可根据需要调整周期
        }
    }
};




int sc_main(int argc, char* argv[])
{
    Functional_bus                functional_bus("Func");
    Power_bus                     power_bus("Power");
    Core                          core("Master");
    Core_power                    core_power("Master_Power");
    core_power.core=&core;
    core_converter core_conv("Master Power Conv");
    input_generator input_generator("input_generator");
    active_suspension_module active_suspension_module ("active_suspension_module");
    plotter plotter ("plotter");
    To_core To_core ("To_core");
    From_core From_core ("From_core");

  
    additional_energy_cost additional_energy_cost ("additional_energy_cost");
   
   
  
   
    
    //sca_tdf::sca_tdf_to_sc<double> tdf2de;
    sc_signal <double> core_state;
    //Master Power Signal
    //sca_tdf::sca_signal <double> sig1;
    sc_signal<double> sig,sig1,sig3,sig4,sig5,sig6,sig7,sig8,sig9,sig10,sig11,sig12,sig13,sig14,sig15,sig16,sig22,sig23,sig24;
    sca_tdf::sca_signal<double> sig2,Thrust,required_rotor_speed,required_current,PID_I,error,signalforce,fs_rec;
    sca_tdf::sca_signal<double> xb_sig, sd_sig, ab_sig, xw_sig,one, fs_sig, r_sig;
    input_generator.out(r_sig);
    input_generator.out1(fs_sig);
    From_core.in(sig1);
    From_core.in1(sig22);
    From_core.in2(sig23);
    From_core.in3(sig24);
    //From_core.out(sig);
  
    core.out(sig1);
    core.out1(sig22);
    core.out2(sig23);
    core.out3(sig24);
  
    To_core.out(sig);
    To_core.out1(sig3);
    To_core.out2(sig15);
    To_core.out3(sig16);

    
    core.in(sig);
    core.in1(sig3);
    core.in2(sig15);
    core.in3(sig16);

    active_suspension_module.r_in(r_sig);
    active_suspension_module.fs_in(fs_sig);
    active_suspension_module.xb_out(xb_sig);
    active_suspension_module.sd_out(sd_sig);
    active_suspension_module.ab_out(ab_sig);
    active_suspension_module.xw_out(xw_sig);
  
    plotter.xb_in(xb_sig);
    plotter.sd_in(sd_sig);
    plotter.ab_in(ab_sig);
    plotter.out1(fs_rec);
   
    
    //core_conv.motor_current(required_current);
    
    
    sca_tdf::sca_signal <double> voltage_core;
    sca_tdf::sca_signal <double> current_core;
    sca_tdf::sca_signal <double> core_conv_current;
    //Functional Bus
    sc_signal <bool> enable_temp;
    //Data from Master to Functional Bus
    sc_signal <unsigned int>  core_request_address;
    sc_signal <uint8_t*>  core_request_data;
    sc_signal <unsigned int>  core_request_size;
    sc_signal <bool> core_request_ready;
    sc_signal <bool> core_functional_bus_flag;
    //Data from Functional Bus to Master
    sc_signal <uint8_t*>  core_request_value;
    sc_signal <bool> core_request_go;
    sc_signal <int> idx_sensor;
    //Data from Functional Bus to Slave
    sc_signal <unsigned int>  address_to_sensors[NUM_SENSORS];
    sc_signal <unsigned int>  size_to_sensors[NUM_SENSORS];
    sc_signal <uint8_t*>  data_to_sensors[NUM_SENSORS];
    sc_signal <bool> F_B_to_S[NUM_SENSORS];
    sc_signal <bool> ready_to_sensors[NUM_SENSORS];

    //Power Bus
    //Signals from Slave to Bus
    sca_tdf::sca_signal <double> voltage_sensors[NUM_SENSORS];
    sca_tdf::sca_signal <double> current_sensors[NUM_SENSORS];
    #if NUM_SOURCES>0
    sca_tdf::sca_signal <double> current_sources[NUM_SOURCES];
    #endif
    #if NUM_BATTERIES>0
    sca_tdf::sca_signal <double> current_batteries[NUM_BATTERIES];
    #endif
    //Output Traces from Power Bus
    sca_tdf::sca_signal <double> overall_voltage;

    Harvester_battery battery("battery");
    battery_converter battery_conv("battery_conv");
    sca_tdf::sca_signal <double> SoC_battery;
    battery.soc(SoC_battery);
    sca_tdf::sca_signal <double> voltage_battery,I_add,I_add2,I_add3;
    sca_tdf::sca_signal <double> current_battery;
    sca_tdf::sca_signal <double> current_battery_bus;
    battery.v(voltage_battery);
    battery.i(current_battery_bus);
    battery_conv.current_in(current_battery);
    battery_conv.voltage_in(voltage_battery);
    battery_conv.current_out(current_battery_bus);
   
  
    Sensor_mic_click_functional mic_click("mic_click");
    mic_click.core=&core;
    Sensor_mic_click_power mic_click_power("mic_click_power");
    // sensors don't support a real converter currently
    Load_converter mic_click_conv("mic_click_converter");
    mic_click_conv.set_efficency(1.0);

    sc_signal <uint8_t*> mic_click_Data;
    sc_signal <bool> mic_click_Go;  
    sc_signal <int> mic_click_F_to_P;
    sca_tdf::sca_signal <double> mic_click_V_State;
    sca_tdf::sca_signal <double> mic_click_I_State;
    sca_tdf::sca_signal <double> mic_click_I_S_to_C;

    mic_click.enable(enable_temp);
    mic_click.address(address_to_sensors[0]);
    mic_click.req_size(size_to_sensors[0]);
    mic_click.data_in(data_to_sensors[0]);
    mic_click.flag_wr(F_B_to_S[0]);
    mic_click.ready(ready_to_sensors[0]);
    mic_click.data_out(mic_click_Data);
    mic_click.go(mic_click_Go);
    mic_click.power_signal(mic_click_F_to_P);
    mic_click_power.func_signal(mic_click_F_to_P);
    mic_click_power.voltage_state(voltage_sensors[0]);
    mic_click_power.current_state(mic_click_I_S_to_C);
    mic_click_conv.current_in(mic_click_I_S_to_C);
    mic_click_conv.voltage_in(voltage_sensors[0]);
    mic_click_conv.current_out(current_sensors[0]);
    functional_bus.data_input_sensor[0](mic_click_Data);
    functional_bus.go_sensors[0](mic_click_Go);

    //Binding Functional Master's signals
    core.request_address(core_request_address);
    core.request_data(core_request_data);
    core.request_size(core_request_size);
    core.request_ready(core_request_ready);
    core.request_go(core_request_go);
    core.request_value(core_request_value);
    core.idx_sensor(idx_sensor);
    core.functional_bus_flag(core_functional_bus_flag);
    core.power_signal(core_state);
    //Binding Power Master's signals
    core_power.func_signal(core_state);
    core_power.voltage_state(voltage_core);
    core_power.current_state(current_core);
    core_power.out(PID_I);
    core_conv.current_in(current_core);
    core_power.out1(I_add);
    core_conv.in(I_add);
    core_conv.out1(I_add2);
    power_bus.in1(I_add2);
    power_bus.out1(I_add3);
    battery_conv.in1(I_add3);
    
   
  
    core_conv.voltage_in(voltage_core);
    core_conv.current_out(core_conv_current);
    //Binding Functional Bus Master's Input 
    functional_bus.request_address(core_request_address);
    functional_bus.request_data(core_request_data);
    functional_bus.request_ready(core_request_ready);
    functional_bus.request_size(core_request_size);
    functional_bus.idx_sensor(idx_sensor);
    functional_bus.flag_from_core(core_functional_bus_flag);
    //Binding Slave Input to Functional Bus

    //Binding Functional Bus's Output Address
    functional_bus.request_value(core_request_value);
    functional_bus.request_go(core_request_go);
    for (size_t i = 0; i < NUM_SENSORS; i++) {
        functional_bus.address_out_sensor[i](address_to_sensors[i]);
        functional_bus.data_out_sensor[i](data_to_sensors[i]);
        functional_bus.flag_out_sensor[i](F_B_to_S[i]);
        functional_bus.size_out_sensor[i](size_to_sensors[i]);
        functional_bus.ready_sensor[i](ready_to_sensors[i]);
        power_bus.voltage_sensors[i](voltage_sensors[i]);
        power_bus.current_sensors[i](current_sensors[i]);
    }
    power_bus.current_batteries[0](current_battery);
    //Binding Power Bus's Input Signal
    power_bus.core_voltage(voltage_core);
    power_bus.core_current(core_conv_current);

    additional_energy_cost.out(required_current);
    core_power.motor_current(required_current);    //here!


     
    // define trace file
    sca_util::sca_trace_file* messy_trace = sca_util::sca_create_tabular_trace_file("messy_trace.log");
    messy_trace->set_mode(sca_sampling(1,SC_MS));
    sca_util::sca_trace_file* va = sca_util::sca_create_tabular_trace_file("va.log");
    va->set_mode(sca_sampling(1,SC_MS));
    sca_util::sca_trace_file* cs = sca_util::sca_create_tabular_trace_file("cs.log");
    cs->set_mode(sca_sampling(1,SC_MS));
    sca_util::sca_trace(cs,xw_sig , "r");
    sca_util::sca_trace(va,sig22, "R");
    sca_util::sca_trace(messy_trace, sig1, "gamma");

  
    //sca_util::sca_trace(messy_trace, SoC_battery, "SOC");
    //sca_util::sca_trace(messy_trace, current_core, "Ic");
    
    enable_temp.write(true);
    std::cout <<"The simulation starts!"<<endl;

    sc_start(SIM_LEN , SC_MS); // Set the simulation length

    std::cout <<"The simulation ends @ "<<sc_time_stamp()<<endl;

    //Close Trace Files
    sca_util::sca_close_tabular_trace_file(messy_trace);

    return 0;
}
