#include <iostream>
#include <string>
#include <systemc>
#include <sys/time.h>
#include "System.h"
#include "Compile_def.h"

#ifndef NATIVE_SYSTEMC
    #include "esc.h"
#endif

using namespace std;
using namespace sc_core;
using namespace sc_dt;

System* sys = NULL;

#ifndef NATIVE_SYSTEMC

    extern void esc_elaborate() {
        sys = new System("sys");
    }

    extern void  esc_cleanup() {
        delete sys;
    }
#endif

int sc_main(int argc, char *argv[]) {

#ifndef NATIVE_SYSTEMC
    esc_initialize(argc, argv);
    esc_elaborate();
#else
    sys = new System("sys");
#endif

    sc_start();

#ifndef NATIVE_SYSTEMC
	esc_cleanup();
#else
	delete sys;
#endif
    std::cout<< "Simulated time == " << sc_core::sc_time_stamp() << std::endl;
    return (0);
}