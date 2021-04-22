#include "Testbench.h"
#include <systemc>

Testbench::Testbench(sc_module_name n): sc_module(n){
  SC_THREAD(input_data);
  sensitive << i_clk.pos();
  dont_initialize();
  SC_THREAD(output_data);
  sensitive << i_clk.pos();
  dont_initialize();
}

int Testbench::read_bmp(){
    FILE* fp_s = fopen("input.bmp", "rb");
    if (fp_s == NULL) {
        std::cerr << "fopen fp_s error" << std::endl;
        return -1;
    }

    fseek(fp_s, 10, SEEK_SET);
    assert(fread(&rgb_raw_data_offset, sizeof(unsigned int), 1, fp_s));

    fseek(fp_s, 18, SEEK_SET);
    assert(fread(&width, sizeof(unsigned int), 1, fp_s));
    assert(fread(&height, sizeof(unsigned int), 1, fp_s));

    fseek(fp_s, 28, SEEK_SET);
    assert(fread(&bit_per_pixel, sizeof(unsigned short), 1, fp_s));
    byte_per_pixel = bit_per_pixel / 8;

    fseek(fp_s, rgb_raw_data_offset, SEEK_SET);

    size_t size = width * height * byte_per_pixel;
    image_s = reinterpret_cast<unsigned char *>(new void *[size]);
    if (image_s == NULL) {
        std::cerr << "allocate image_s error" << std::endl;
        return -1;
    }

    image_t = reinterpret_cast<unsigned char *>(new void *[size]);
    if (image_t == NULL) {
        std::cerr << "allocate image_t error" << std::endl;
        return -1;
    }

    assert(fread(image_s, sizeof(unsigned char),
                (size_t)(long)width * height * byte_per_pixel, fp_s));
    fclose(fp_s);

    return 0;
}
int Testbench::write_bmp(){
    unsigned int file_size = 0; // file size
    FILE* fp_t = fopen("output.bmp", "wb");
    if (fp_t == NULL) {
        std::cerr << "fopen fname_t error" << std::endl;
        return -1;
    }
    file_size = width * height * byte_per_pixel + rgb_raw_data_offset;
    header[2] = (unsigned char)(file_size & 0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) & 0x000000ff;
    header[20] = (width >> 16) & 0x000000ff;
    header[21] = (width >> 24) & 0x000000ff;
    header[22] = height & 0x000000ff;
    header[23] = (height >> 8) & 0x000000ff;
    header[24] = (height >> 16) & 0x000000ff;
    header[25] = (height >> 24) & 0x000000ff;
    header[28] = bit_per_pixel;
    fwrite(header, sizeof(unsigned char), rgb_raw_data_offset, fp_t);
    fwrite(image_t, sizeof(unsigned char),
            (size_t)(long)width * height * byte_per_pixel, fp_t);

    fclose(fp_t);

    return 0;
}
void Testbench::input_data(){
    int offset = 0;
    int i, j, x, y;
    int filterHeight = 3, filterWidth = 3;
    unsigned char R, G, B;      // color of R, G, B

#ifndef NATIVE_SYSTEMC
	o_r.reset();
	o_g.reset();
	o_b.reset();
#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);

    for (y = 0; y != height; ++y) {
        for (x = 0; x != width; ++x) {
            for (i=-1 ; i<filterHeight-1 ; ++i) {
                for (j=-1 ; j<filterWidth-1 ; ++j) {
                    if(0<=y+i && y+i<height && 0<=x+j && x+j<width) {
                        R = *(image_s + byte_per_pixel * (width * (y+i) + x + j + offset) + 2);
						G = *(image_s + byte_per_pixel * (width * (y+i) + x + j + offset) + 1);
						B = *(image_s + byte_per_pixel * (width * (y+i) + x + j + offset) + 0);
                    }
					else{
 						R = 0;
						G = 0;
						B = 0;
					}
#ifndef NATIVE_SYSTEMC
					o_r.put(R);
					o_g.put(G);
					o_b.put(B);
#else
					o_r.write(R);
					o_g.write(G);
					o_b.write(B);
#endif
                }
            }
        }
    }

}
void Testbench::output_data(){
	int x, y;
	sc_uint<8> R, G, B;

#ifndef NATIVE_SYSTEMC
	i_r.reset();
	i_g.reset();
	i_b.reset();
#endif

	wait(1);

	for (y = 0; y != height; ++y) {
		for (x = 0; x != width; ++x) {
#ifndef NATIVE_SYSTEMC
			R = i_r.get();
			G = i_g.get();
			B = i_b.get();
#else
		    R = i_r.read();
			G = i_g.read();
			B = i_b.read();
#endif
			*(image_t + byte_per_pixel * (width * y + x) + 2) = R;
			*(image_t + byte_per_pixel * (width * y + x) + 1) = G;
			*(image_t + byte_per_pixel * (width * y + x) + 0) = B;
		}
	}
    sc_stop();
}