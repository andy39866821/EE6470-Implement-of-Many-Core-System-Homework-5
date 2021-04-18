#include "System.h"
System::System( sc_module_name n): sc_module( n ), 
	tb("tb"), filter("filter"), clk("clk", CLOCK_PERIOD, SC_NS), rst("rst")
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	filter.i_clk(clk);
	filter.i_rst(rst);
	tb.o_rgb(source_rgb);
    tb.i_rgb(result_rgb);
    filter.i_rgb(source_rgb);
    filter.o_rgb(result_rgb);

    cout << "Start reading bitmap" << endl;
    tb.read_bmp();
    
    cout << "Finish reading bitmap" << endl;
}

System::~System() {
    
    cout << "Start writing bitmap" << endl;
    tb.write_bmp();
    cout << "Finish writing bitmap" << endl;
}

    
    