#include <systemc>
#include <sys/time.h> 
#include "Compile_def.h"

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

using namespace std;
using namespace sc_core;
using namespace sc_dt;

class Filter: public sc_module {
private:
    void convolution();
public:

	sc_in_clk i_clk;
	sc_in < bool > i_rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<24> >::in i_rgb;
	cynw_p2p< sc_dt::sc_uint<24> >::out o_rgb;
#else
    sc_fifo_out<sc_uint<24> > o_rgb;
    sc_fifo_in<sc_uint<24> > i_rgb;
#endif
    SC_HAS_PROCESS(Filter);
    Filter(sc_module_name n);
    ~Filter();
   
};