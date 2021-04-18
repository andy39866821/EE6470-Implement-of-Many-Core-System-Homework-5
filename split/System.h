#ifndef SYSTEM_H_
#define SYSTEM_H_
#include <systemc>
#include <string>
#include "Compile_def.h"
#include "Testbench.h"

#ifndef NATIVE_SYSTEMC
    #include "Filter_wrap.h"
#else
    #include "Filter.h"
#endif

using namespace sc_core;


class System: public sc_module
{
public:
	SC_HAS_PROCESS( System );
	System( sc_module_name n);
	~System();
private:

  	Testbench tb;
#ifndef NATIVE_SYSTEMC
	Filter_wrapper filter;
#else
	Filter filter;
#endif
	sc_clock clk;
	sc_signal<bool> rst;
    

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

};
#endif
