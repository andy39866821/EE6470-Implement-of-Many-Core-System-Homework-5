
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
	i_rgb.clk_rst(i_clk, i_rst);
    o_rgb.clk_rst(i_clk, i_rst);
#endif

}

Filter::~Filter() {}

void Filter::convolution(){
    sc_uint<8> source_r, source_g, source_b, i ,j;
    sc_uint<24> source_rgb, result_rgb;
    sc_uint<32>  R, G, B;      // color of R, G, B
	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_rgb.reset();
		o_rgb.reset();
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
					source_rgb = i_rgb.get();
                    wait();
				}
#else
                source_rgb = i_rgb.read();
#endif
                source_r = source_rgb.range(7,0);
                source_g = source_rgb.range(15,8);
                source_b = source_rgb.range(23,16);
                {
                    HLS_CONSTRAIN_LATENCY(0, 1, "lat01");
                    R += (sc_uint<32>)source_r * filter[i][j];
                    G += (sc_uint<32>)source_g * filter[i][j];
                    B += (sc_uint<32>)source_b * filter[i][j];
                }
            }
        }
        result_rgb.range(7,0) = (sc_uint<8>)(R / factor);
        result_rgb.range(15,8) = (sc_uint<8>)(G / factor);
        result_rgb.range(23,16) = (sc_uint<8>)(B / factor);
#ifndef NATIVE_SYSTEMC
		{
			HLS_DEFINE_PROTOCOL("output");
            o_rgb.put(result_rgb);
			wait();
		}
#else
        o_rgb.write(result_rgb);
#endif
    }
    
}
