#include "System.h"
System::System( sc_module_name n): sc_module( n ), 
	tb("tb"), filter("filter"), clk("clk", CLOCK_PERIOD, SC_NS), rst("rst")
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	filter.i_clk(clk);
	filter.i_rst(rst);
	tb.o_r(source_r);
    tb.o_g(source_g);
    tb.o_b(source_b);
    tb.i_r(result_r);
    tb.i_g(result_g);
    tb.i_b(result_b);
    filter.i_r(source_r);
    filter.i_g(source_g);
    filter.i_b(source_b);
    filter.o_r(result_r);
    filter.o_g(result_g);
    filter.o_b(result_b);

    cout << "Start reading bitmap" << endl;
    tb.read_bmp();
    
    cout << "Finish reading bitmap" << endl;
}

System::~System() {
    
    cout << "Start writing bitmap" << endl;
    tb.write_bmp();
    cout << "Finish writing bitmap" << endl;
}

    
    