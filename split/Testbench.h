#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <string>
#include <systemc>
#include <iostream>
#include "Compile_def.h"

#ifndef NATIVE_SYSTEMC
    #include <cynw_p2p.h>
#endif

using namespace std;
using namespace sc_core;
using namespace sc_dt;

class Testbench: public sc_module {
private:

    unsigned char *image_s = NULL; // source image array
    unsigned char *image_t = NULL; // target image array
    int width = 0;               // image width
    int height = 0;              // image height
    unsigned int rgb_raw_data_offset = 0; // RGB raw data offset
    unsigned int bit_per_pixel = 0;      // bit per pixel
    unsigned int byte_per_pixel = 0;    // byte per pixel
    unsigned char header[54] = {
        0x42,          // identity : B
        0x4d,          // identity : M
        0,    0, 0, 0, // file size
        0,    0,       // reserved1
        0,    0,       // reserved2
        54,   0, 0, 0, // RGB data offset
        40,   0, 0, 0, // struct BITMAPINFOHEADER size
        0,    0, 0, 0, // bmp width
        0,    0, 0, 0, // bmp height
        1,    0,       // planes
        24,   0,       // bit per pixel
        0,    0, 0, 0, // compression
        0,    0, 0, 0, // data size
        0,    0, 0, 0, // h resolution
        0,    0, 0, 0, // v resolution
        0,    0, 0, 0, // used colors
        0,    0, 0, 0  // important colors
    };

    void input_data();
    void output_data();

public:

	sc_in_clk i_clk;
	sc_out < bool >  o_rst;
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
    SC_HAS_PROCESS(Testbench);
    Testbench(sc_module_name n);
    ~Testbench() = default;
    int read_bmp();
    int write_bmp();
};

#endif