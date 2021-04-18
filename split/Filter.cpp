
#ifndef NATIVE_SYSTEMC
    #include "stratus_hls.h"
#endif  
#include "Filter.h"

const int filter[3][3] = {
    {1,2,1},
    {2,4,2},
    {1,2,1}
};
const int factor = 16;

Filter::Filter(sc_module_name n): 
    sc_module(n)
{
    SC_THREAD( convolution );
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);
#ifndef NATIVE_SYSTEMC
	i_r.clk_rst(i_clk, i_rst);
	i_g.clk_rst(i_clk, i_rst);
	i_b.clk_rst(i_clk, i_rst);
    o_r.clk_rst(i_clk, i_rst);
    o_g.clk_rst(i_clk, i_rst);
    o_b.clk_rst(i_clk, i_rst);
#endif

}

Filter::~Filter() {}

void Filter::convolution(){
    sc_uint<8> source_r, source_g, source_b, i ,j;
    sc_uint<32>  R, G, B;      // color of R, G, B
	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_r.reset();
		i_g.reset();
		i_b.reset();
		o_r.reset();
		o_g.reset();
		o_b.reset();
#endif
		wait();
	}
    while(true) {
        R = G = B = 0;
        for (i=0 ; i!=3 ; ++i) {
            for (j=0 ; j!=3 ; ++j) {
#ifndef NATIVE_SYSTEMC
                //HLS_PIPELINE_LOOP( HARD_STALL,18, "main_loop_pipeline" );
        
				{
					HLS_DEFINE_PROTOCOL("input");
					source_r = i_r.get();
                    source_g = i_g.get();
                    source_b = i_b.get();
                    wait();
				}
#else
                source_r = i_r.read();
                source_g = i_g.read();
                source_b = i_b.read();
#endif

                {
                    HLS_CONSTRAIN_LATENCY(0, 1, "lat01");
                    R += (sc_uint<32>)source_r * filter[i][j];
                    G += (sc_uint<32>)source_g * filter[i][j];
                    B += (sc_uint<32>)source_b * filter[i][j];
                }
            }
        }

#ifndef NATIVE_SYSTEMC
		{
			HLS_DEFINE_PROTOCOL("output");
            o_r.put((sc_uint<8>)(R/factor));
            o_g.put((sc_uint<8>)(G/factor));
            o_b.put((sc_uint<8>)(B/factor));
			wait();
		}
#else
        o_r.write((sc_uint<8>)(R/factor));
        o_g.write((sc_uint<8>)(G/factor));
        o_b.write((sc_uint<8>)(B/factor));
#endif
    }
    
}
