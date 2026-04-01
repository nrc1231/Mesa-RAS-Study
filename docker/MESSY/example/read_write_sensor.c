/* PMSIS includes */
#include "pmsis.h"
#include <stdio.h>
//#include <cstdlib>

#ifdef GAP_SDK
#define AXI_BASE 0x80000000
#else
#define AXI_BASE 0x20000000
#endif
#define NUM_ITERS 50000
/* Program Entry. */
double theta1=0, theta2=0;   
double gamma1=250, gamma2=100,error=0,prev_a=0;   //250,200
double dt=0.001;               
double kp = 12;//12
double cs=0;
int va=0;
double sc=0;
int z=0,vy=0,xw=0,vw=0,arlr=0,arrr=0;
int i=0;
int memory=0;
double Rvar=0;

int main(void)
{
    printf("\n\n\t *** PMSIS HelloWorld ***\n\n");
    printf("Entering main controller\n");
    
    uint32_t errors = 10;
    uint32_t core_id = pi_core_id(), cluster_id = pi_cluster_id();
    printf("[%d %d] Hello World!\n", cluster_id, core_id);

  
    int* mic_click_sensor1 = (volatile int *)(AXI_BASE+0x0); 
    int* mic_click_sensor2 = (volatile int *)(AXI_BASE+0x4); 
    int* mic_click_sensor4 = (volatile int *)(AXI_BASE+0x10); 
    int* mic_click_sensor5 = (volatile int *)(AXI_BASE+0x14); 
    int* mic_click_sensor6 = (volatile int *)(AXI_BASE+0x20); 
    int* mic_click_sensor7 = (volatile int *)(AXI_BASE+0x24); 
    int* mic_click_sensor8 = (volatile int *)(AXI_BASE+0x30); 
    int* mic_click_sensor9 = (volatile int *)(AXI_BASE+0x34); 
  
    for(int i=0;i<NUM_ITERS;i++){
    
       i++;
       
       z=* mic_click_sensor1;
       vy=* mic_click_sensor2;
       arlr=* mic_click_sensor4;
       arrr=* mic_click_sensor7;
      
       double v_s=z;
       double v_rel=vy;
       double ec=vw;
       double arl=arlr;
       double arr=arrr;
       v_rel=v_rel/1000000;
       v_s=v_s/1000000;
       arl=arl/1000000;
       arr=arr/1000000;
       static double sigma=1;

    double fdes  = 0.0, fdes2 = 0.0, fdes3 = 0.0, fdes4 = 0.0;
    double fdes12  = 0.0, fdes22 = 0.0, fdes23 = 0.0, fdes24 = 0.0;
    double fdes13  = 0.0, fdes32 = 0.0, fdes33 = 0.0, fdes34 = 0.0;
    double fdes14  = 0.0, fdes42 = 0.0, fdes43 = 0.0, fdes44 = 0.0;
    
    double error_vs   = v_s;
    double error_vrel = v_rel;
    double error_rl   = arl;
    double error_rr   = arr;
    static double Ides=0,Ides2=0,Ides3=0,Ides4=0;
    
    static double K_vs   = 105.0, KI_vs   = 0.350, KD_vs   = 1.2;
    static double K_vrel = 105.0, KI_vrel = 0.350, KD_vrel = 1.2;
    static double K_rl   = 105.0, KI_rl   = 0.350, KD_rl   = 1.2;
    static double K_rr   = 105.0, KI_rr   = 0.350, KD_rr   = 1.2;

    
    const double gammaP = 1.2e-4;    // P2e-4
    const double gammaI = 1e-4;    // I 5e-5
    const double gammaD = 4e-4;    // D 5e-5

    
    const double leakP  = 1e-4;
    const double leakI  = 1e-6;
    const double leakD  = 1e-4;

    const double K_MIN  = 95.0,  K_MAX  = 125.0;
    const double KI_MIN = 0.3,  KI_MAX = 0.4;
    const double KD_MIN = 0.0,  KD_MAX = 10.0;

    
    const double alpha_d = 0.9;

    
    static double integ_vs   = 0.0, prev_err_vs   = 0.0, d_filt_vs   = 0.0;
    static double integ_vrel = 0.0, prev_err_vrel = 0.0, d_filt_vrel = 0.0;
    static double integ_rl   = 0.0, prev_err_rl   = 0.0, d_filt_rl   = 0.0;
    static double integ_rr   = 0.0, prev_err_rr   = 0.0, d_filt_rr   = 0.0;

    
    {
        Ides += v_s;
        double deriv = (error_vs - prev_err_vs);
        d_filt_vs = alpha_d * d_filt_vs + (1.0 - alpha_d) * deriv;
        double d_use = d_filt_vs;

    
        K_vs  += gammaP * (error_vs * error_vs)         - leakP * K_vs;
        KI_vs += gammaI * (integ_vs * integ_vs)         - leakI * KI_vs;
        KD_vs += gammaD * (deriv * deriv)               - leakD * KD_vs;

        if (K_vs  < K_MIN ) K_vs  = K_MIN;  if (K_vs  > K_MAX ) K_vs  = K_MAX;
        if (KI_vs < KI_MIN) KI_vs = KI_MIN; if (KI_vs > KI_MAX) KI_vs = KI_MAX;
        if (KD_vs < KD_MIN) KD_vs = KD_MIN; if (KD_vs > KD_MAX) KD_vs = KD_MAX;

        fdes12 =K_vs * v_s + KI_vs * Ides + KD_vs * d_use;  //PIDAD
        fdes13= 100*v_s+0.25*Ides;                          //PI
        fdes14= 1440*v_s;                                   //SH
        fdes=fdes13;               //choose the controller you want to test
        prev_err_vs = error_vs;
    }

    
    {
        Ides2 += v_rel;
        double deriv = (error_vrel - prev_err_vrel);
        d_filt_vrel = alpha_d * d_filt_vrel + (1.0 - alpha_d) * deriv;
        double d_use = d_filt_vrel;

        K_vrel  += gammaP * (error_vrel * error_vrel)          - leakP * K_vrel;
        KI_vrel += gammaI * (integ_vrel * integ_vrel)          - leakI * KI_vrel;
        KD_vrel += gammaD * (deriv * deriv)                    - leakD * KD_vrel;

        if (K_vrel  < K_MIN ) K_vrel  = K_MIN;  if (K_vrel  > K_MAX ) K_vrel  = K_MAX;
        if (KI_vrel < KI_MIN) KI_vrel = KI_MIN; if (KI_vrel > KI_MAX) KI_vrel = KI_MAX;
        if (KD_vrel < KD_MIN) KD_vrel = KD_MIN; if (KD_vrel > KD_MAX) KD_vrel = KD_MAX;

        fdes22 =K_vrel * v_rel + KI_vrel * Ides2 + KD_vrel * d_use;
        fdes32=100*v_rel+0.25*Ides2;
        fdes42=1440*v_rel;
        fdes2=fdes32;
        prev_err_vrel = error_vrel;
    }

    
    {
        Ides3 += arl;
        double deriv = (error_rl - prev_err_rl);
        d_filt_rl = alpha_d * d_filt_rl + (1.0 - alpha_d) * deriv;
        double d_use = d_filt_rl;

        K_rl  += gammaP * (error_rl * error_rl)          - leakP * K_rl;
        KI_rl += gammaI * (integ_rl * integ_rl)          - leakI * KI_rl;
        KD_rl += gammaD * (deriv * deriv)                - leakD * KD_rl;

        if (K_rl  < K_MIN ) K_rl  = K_MIN;  if (K_rl  > K_MAX ) K_rl  = K_MAX;
        if (KI_rl < KI_MIN) KI_rl = KI_MIN; if (KI_rl > KI_MAX) KI_rl = KI_MAX;
        if (KD_rl < KD_MIN) KD_rl = KD_MIN; if (KD_rl > KD_MAX) KD_rl = KD_MAX;

        fdes23 =K_rl * arl + KI_rl * Ides3 + KD_rl * d_use;
        fdes33=100*arl+0.25*Ides3;
        fdes43=1440*arl;
        fdes3=fdes33;
        prev_err_rl = error_rl;
    }

    
    {
        Ides4 += arr;
        double deriv = (error_rr - prev_err_rr);
        d_filt_rr = alpha_d * d_filt_rr + (1.0 - alpha_d) * deriv;
        double d_use = d_filt_rr;

        K_rr  += gammaP * (error_rr * error_rr)          - leakP * K_rr;
        KI_rr += gammaI * (integ_rr * integ_rr)          - leakI * KI_rr;
        KD_rr += gammaD * (deriv * deriv)                - leakD * KD_rr;

        if (K_rr  < K_MIN ) K_rr  = K_MIN;  if (K_rr  > K_MAX ) K_rr  = K_MAX;
        if (KI_rr < KI_MIN) KI_rr = KI_MIN; if (KI_rr > KI_MAX) KI_rr = KI_MAX;
        if (KD_rr < KD_MIN) KD_rr = KD_MIN; if (KD_rr > KD_MAX) KD_rr = KD_MAX;

        fdes24 = K_rr * arr + KI_rr * Ides4 + KD_rr * d_use;
        fdes34=100*arr+0.25*Ides4;
        fdes44=1440*arr;
        fdes4=fdes34;
        prev_err_rr = error_rr;
    }

       fdes=fdes*1000;
       fdes2=fdes2*1000;
       fdes3=fdes3*1000;
       fdes4=fdes4*1000;
       printf("[GvSOC]fdes1 = %f\n",v_s);
       
       
        if(i==0){                    
          *mic_click_sensor1 = 0;
          *mic_click_sensor2 = 0;
          *mic_click_sensor4 = 0;
          *mic_click_sensor5 = 0;
          *mic_click_sensor6 = 0;
          *mic_click_sensor7 = 0;
          *mic_click_sensor8 = 0;
          *mic_click_sensor9 = 0;
          
    
        }
        else{
          *mic_click_sensor1 = 1;
          *mic_click_sensor2 = 1;
          *mic_click_sensor5 = fdes;
          *mic_click_sensor6 = fdes2;
          *mic_click_sensor4 = 1;
          *mic_click_sensor7 = 1;
          *mic_click_sensor8 = fdes3;
          *mic_click_sensor9 = fdes4;
        }
        
    }

    return errors;
}
    
