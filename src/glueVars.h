#include "iec_std_lib.h"

TIME __CURRENT_TIME;
extern unsigned long long common_ticktime__;

//Internal buffers for I/O and memory. These buffers are defined in the
//auto-generated glueVars.cpp file
#define BUFFER_SIZE		1024

//Booleans
IEC_BOOL *bool_input[BUFFER_SIZE][8];
IEC_BOOL *bool_output[BUFFER_SIZE][8];

//Bytes
IEC_BYTE *byte_input[BUFFER_SIZE];
IEC_BYTE *byte_output[BUFFER_SIZE];

//Analog I/O
IEC_UINT *int_input[BUFFER_SIZE];
IEC_UINT *int_output[BUFFER_SIZE];

//Memory
IEC_UINT *int_memory[BUFFER_SIZE];
IEC_DINT *dint_memory[BUFFER_SIZE];
IEC_LINT *lint_memory[BUFFER_SIZE];

//Special Functions
IEC_LINT *special_functions[BUFFER_SIZE];


extern void glueVars();
extern void updateTime();