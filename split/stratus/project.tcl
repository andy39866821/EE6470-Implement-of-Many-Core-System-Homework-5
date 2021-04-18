

set LIB_PATH "[get_install_path]/share/stratus/techlibs/GPDK045/gsclib045_svt_v4.4/gsclib045/timing"
set LIB_LEAF "slow_vdd1v2_basicCells.lib"
use_tech_lib    "$LIB_PATH/$LIB_LEAF"
 
set_attr clock_period 5.0 
set_attr default_input_delay 0.1 
 
# message level
set_attr message_detail 2
 
# set dump settin
use_verilog_simulator incisive
set_attr cc_options             " -g" 
enable_waveform_logging -fsdb 
set_attr end_of_sim_command "make saySimPassed"
 
define_system_module ../main.cpp
define_system_module ../System.cpp
define_system_module ../Testbench.cpp
 
define_hls_module Filter ../Filter.cpp 

define_hls_config Filter BASIC    
define_hls_config Filter DPA       --dpopt_auto=op,expr
 
### 5. Define simulation configuration for each HLS configuration
### 5.1 The behavioral simulation (C++ only).
define_sim_config B 
### 5.2 The Verilog simulation for HLS config "BASIC". 
define_sim_config V_BASIC "Filter RTL_V BASIC" 
### 5.3 The Verilog simulation for HLS config "DPA". 
define_sim_config V_DPA "Filter RTL_V DPA"