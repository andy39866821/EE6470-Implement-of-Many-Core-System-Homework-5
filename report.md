# EE6470 Homework 5 Report
###### tags: `EE6470`
## Problems and Solutions
### HLS command
Q: At first I don't know the scripts in Makefile and how to integrate it with project.tcl
A: First generate Makefile.prj by below command, then use the command to simulate or HLS the systemc code.
```
bdw_makegen project.tcl
make sim_B
make sim_V_BASIC
make sim_V_DPA
```
### **NATIVE_SYSTEMC** defination for HLS
Q: HLS failed with datatype
![](https://i.imgur.com/Gq75kbZ.jpg)

A: In **make sim_B**, it should use TLM datatype to simulate, where we should define **NATIVE_SYSTEMC**, but in **make sim_V_BASIC** and **make sim_V_DPA**, it should use systhesizable datatype, where we should not define **NATIVE_SYSTEMC**.

## Implementation details 

### Embedding synthesisable code for HLS
For every file in this homework, I add the ifdef statement to replace I/O port data type for HLS:
* Testbench
    * Non-split
    ```
    #ifndef NATIVE_SYSTEMC
        cynw_p2p< sc_dt::sc_uint<24> >::base_out o_rgb;
        cynw_p2p< sc_dt::sc_uint<24> >::base_in i_rgb;
    #else
        sc_fifo_out<sc_uint<24> > o_rgb;
        sc_fifo_in<sc_uint<24> > i_rgb;
    #endif
    ```
    * Split
    ```
    #ifndef NATIVE_SYSTEMC
        cynw_p2p< sc_dt::sc_uint<8> >::base_out o_r;
        cynw_p2p< sc_dt::sc_uint<8> >::base_out o_g;
        cynw_p2p< sc_dt::sc_uint<8> >::base_out o_b;
        cynw_p2p< sc_dt::sc_uint<8> >::base_in i_r;
        cynw_p2p< sc_dt::sc_uint<8> >::base_in i_g;
        cynw_p2p< sc_dt::sc_uint<8> >::base_in i_b;
    #else
        sc_fifo_out<sc_uint<8> > o_r;
        sc_fifo_out<sc_uint<8> > o_g;
        sc_fifo_out<sc_uint<8> > o_b;
        sc_fifo_in<sc_uint<8> > i_r;
        sc_fifo_in<sc_uint<8> > i_g;
        sc_fifo_in<sc_uint<8> > i_b;
    #endif    
    ```
* System
    * Non-split
    ```
    #ifndef NATIVE_SYSTEMC
        cynw_p2p< sc_dt::sc_uint<24> > source_rgb;
        cynw_p2p< sc_dt::sc_uint<24> > result_rgb;
    #else
        sc_fifo<sc_uint<24> > source_rgb;
        sc_fifo<sc_uint<24> > result_rgb;
    #endif        
    ```
    * Split
    ```
    #ifndef NATIVE_SYSTEMC
        cynw_p2p< sc_dt::sc_uint<8> > source_r;
        cynw_p2p< sc_dt::sc_uint<8> > source_g;
        cynw_p2p< sc_dt::sc_uint<8> > source_b;
        cynw_p2p< sc_dt::sc_uint<8> > result_r;
        cynw_p2p< sc_dt::sc_uint<8> > result_g;
        cynw_p2p< sc_dt::sc_uint<8> > result_b;
    #else
        sc_fifo<sc_uint<8> > source_r;
        sc_fifo<sc_uint<8> > source_g;
        sc_fifo<sc_uint<8> > source_b;
        sc_fifo<sc_uint<8> > result_r;
        sc_fifo<sc_uint<8> > result_g;
        sc_fifo<sc_uint<8> > result_b;
    #endif
    ```
* Filter
    * Non-split
    ```
    #ifndef NATIVE_SYSTEMC
        cynw_p2p< sc_dt::sc_uint<24> >::in i_rgb;
        cynw_p2p< sc_dt::sc_uint<24> >::out o_rgb;
    #else
        sc_fifo_out<sc_uint<24> > o_rgb;
        sc_fifo_in<sc_uint<24> > i_rgb;
    #endif
    ```
    * Split
    ```
    #ifndef NATIVE_SYSTEMC
        cynw_p2p< sc_dt::sc_uint<8> >::in i_r;
        cynw_p2p< sc_dt::sc_uint<8> >::in i_g;
        cynw_p2p< sc_dt::sc_uint<8> >::in i_b;
        cynw_p2p< sc_dt::sc_uint<8> >::out o_r;
        cynw_p2p< sc_dt::sc_uint<8> >::out o_g;
        cynw_p2p< sc_dt::sc_uint<8> >::out o_b;
    #else
        sc_fifo_out<sc_uint<8> > o_r;
        sc_fifo_out<sc_uint<8> > o_g;
        sc_fifo_out<sc_uint<8> > o_b;
        sc_fifo_in<sc_uint<8> > i_r;
        sc_fifo_in<sc_uint<8> > i_g;
        sc_fifo_in<sc_uint<8> > i_b;
    #endif
    ```
In filter design code, I add specific I/O command for **cynw_p2p** datatype:
```
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_r.reset();
		i_g.reset();
		i_b.reset();
		o_r.reset();
		o_g.reset();
		o_b.reset();
#endif
```
```
{
    HLS_DEFINE_PROTOCOL("input");
    source_r = i_r.get();
    source_g = i_g.get();
    source_b = i_b.get();
    wait();
}
```
```
{
    HLS_DEFINE_PROTOCOL("output");
    o_r.put((sc_uint<8>)(R/factor));
    o_g.put((sc_uint<8>)(G/factor));
    o_b.put((sc_uint<8>)(B/factor));
    wait();
}
```

## Additional features
### Simplier defination in source file
I define **NATIVE_SYSTEMC** in **Compile_def.h** but not in Makefile, which may make command not so easy to be understod and modified. And I will comment the define when we need HLS.
```
//#define NATIVE_SYSTEMC
#define CLOCK_PERIOD 10

```

## Experimental results
### Simulating cycle
* split
```
Behaviour    : 60ms
V_BASIC      : 128450.615100 us
V_DPA        : 128450.615100 us
```
* non-split
```
Behaviour    : 60ms
V_BASIC      : 76021.815100 us
V_DPA        : 76021.815100 us
```
For the above result, we can found that if we integrate multiple port into a larger width port can reduce the latency of I/O.

### Resource allocated
* split
```
               +--------------------------------------------------------------------+
               |                                                                    |
        00803: | Allocation Report for all threads:                                 |
        00805: |                                        Area/Instance               |
        00805: |                                  ------------------------    Total |
        00805: |                 Resource  Count    Seq(#FF)    Comb    BB     Area |
        00805: | ------------------------  -----  ----------  ------  ----  ------- |
        00807: |  Filter_Mul_12Sx8U_12S_4      2               374.8          749.7 |
        00807: | Filter_Add_12Ux12U_12U_4      2                91.7          183.3 |
        00807: |             mux_12bx2i1c      3                32.6           97.8 |
        00807: |             mux_12bx2i0c      2                37.3           74.5 |
        00807: |              mux_8bx2i0c      3                24.8           74.5 |
        00807: |      Filter_gen_busy_r_1      3                17.8           53.4 |
        00807: |              mux_8bx3i0c      1                27.0           27.0 |
        00807: |    Filter_And_1Ux1U_1U_1      3                 8.9           26.7 |
        00807: |    Filter_Add_4Ux2U_4U_4      1                15.0           15.0 |
        00807: |    Filter_Xor_1Ux1U_1U_1      3                 4.4           13.3 |
        00807: |       Filter_Not_1U_1U_1      3                 4.1           12.3 |
        00807: |    Filter_Add_2Ux1U_2U_4      2                 6.2           12.3 |
        00807: |    Filter_Mul_2Ux2U_4U_4      1                11.3           11.3 |
        00807: |              mux_2bx2i1c      2                 5.4           10.9 |
        00807: |              mux_4bx2i1c      1                10.9           10.9 |
        00807: |              mux_1bx7i1c      1                 9.5            9.5 |
        00807: |              mux_1bx2i2c      4                 2.3            9.3 |
        00807: |    Filter_N_Muxb_1_2_0_4      3                 2.4            7.2 |
        00807: |     Filter_Or_1Ux1U_1U_4      3                 1.4            4.1 |
        00807: |   Filter_ROM_9X32_filter      1                         ?        ? |
        00808: |                registers     27                                    |
        01442: |        Reg bits by type:                                           |
        01442. |           EN SS SC AS AC                                           |
        00809: |            0  0  1  0  0      6    5.5(1)       1.4                |
        00809: |            0  1  0  0  0      3    5.5(1)       1.4                |
        00809: |            1  0  0  0  0     88    7.5(1)       0.0                |
        00809: |            1  0  1  0  0      8    7.5(1)       1.4                |
        00809: |            1  1  0  0  0      3    7.5(1)       1.4                |
        00809: |        all register bits    108    7.4(1)       0.3          821.5 |
        02604: |          estimated cntrl      1                57.1           57.1 |
        00811: | ------------------------------------------------------------------ |
        00812: |               Total Area         794.1(108)  1487.4   0.0   2281.5 |
               |                                                                    |
               +--------------------------------------------------------------------+
```
* non-split
```
               +--------------------------------------------------------------------+
               |                                                                    |
        00803: | Allocation Report for all threads:                                 |
        00805: |                                        Area/Instance               |
        00805: |                                  ------------------------    Total |
        00805: |                 Resource  Count    Seq(#FF)    Comb    BB     Area |
        00805: | ------------------------  -----  ----------  ------  ----  ------- |
        00807: |  Filter_Mul_12Sx8U_12S_4      2               374.8          749.7 |
        00807: | Filter_Add_12Ux12U_12U_4      2                91.7          183.3 |
        00807: |             mux_12bx2i0c      4                37.3          149.0 |
        00807: |             mux_12bx2i1c      3                32.6           97.8 |
        00807: |              mux_8bx2i0c      2                24.8           49.7 |
        00807: |      Filter_gen_busy_r_1      1                17.8           17.8 |
        00807: |    Filter_Add_4Ux2U_4U_4      1                15.0           15.0 |
        00807: |    Filter_Add_2Ux1U_2U_4      2                 6.2           12.3 |
        00807: |    Filter_Mul_2Ux2U_4U_4      1                11.3           11.3 |
        00807: |              mux_2bx2i1c      2                 5.4           10.9 |
        00807: |    Filter_And_1Ux1U_1U_1      1                 8.9            8.9 |
        00807: |              mux_3bx2i1c      1                 8.1            8.1 |
        00807: |              mux_2bx3i1c      1                 6.7            6.7 |
        00807: |              mux_1bx2i2c      2                 2.3            4.7 |
        00807: |    Filter_Xor_1Ux1U_1U_1      1                 4.4            4.4 |
        00807: |       Filter_Not_1U_1U_1      1                 4.1            4.1 |
        00807: |              mux_1bx3i1c      1                 3.8            3.8 |
        00807: |    Filter_N_Muxb_1_2_0_4      1                 2.4            2.4 |
        00807: |     Filter_Or_1Ux1U_1U_4      1                 1.4            1.4 |
        00807: |   Filter_ROM_9X32_filter      1                         ?        ? |
        00808: |                registers     16                                    |
        01442: |        Reg bits by type:                                           |
        01442. |           EN SS SC AS AC                                           |
        00809: |            0  0  1  0  0      2    5.5(1)       1.4                |
        00809: |            0  1  0  0  0      1    5.5(1)       1.4                |
        00809: |            1  0  0  0  0     96    7.5(1)       0.0                |
        00809: |            1  0  1  0  0      5    7.5(1)       1.4                |
        00809: |            1  1  0  0  0      1    7.5(1)       1.4                |
        00809: |        all register bits    105    7.5(1)       0.1          796.2 |
        02604: |          estimated cntrl      1                48.0           48.0 |
        00811: | ------------------------------------------------------------------ |
        00812: |               Total Area         783.9(105)  1401.5   0.0   2185.4 |
               |                                                                    |
               +--------------------------------------------------------------------+
```
For the above result, we can find that integrate multiple ports can reduce the amount of register
=> lower area

## Discussions and conclusions
For the HLS technique, since HLS can generate FSM if needed in data path module, it is very useful for the pure data path design, which don't need to complicated FSM, the FSM we need is to contorl ready and busy signal of thie module. 
Also HLS simpler the verification time, which will take lots of time for designer to control the timing of I/O data with design module in testbench.
