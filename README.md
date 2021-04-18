# Homework 4
## Compilation & Execution
Makefile has been generated in folder **stratus**.
To run behavior simulation, you need to uncomment **NATIVE_SYSTEMC** in **Comple_def.h**, then run below command.
```
	make sim_B
```
To run HLS and RTL simulation, you need to comment **NATIVE_SYSTEMC** in **Comple_def.h**, then run below command.
```
	make sim_V_BASIC
	make sim_V_DPA
```

## Result
This program will generate bimap file in **stratus**
```
    output.bmp        
```