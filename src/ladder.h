#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>

//Internal buffers for I/O and memory. These buffers are defined in the
//auto-generated glueVars.cpp file
#define BUFFER_SIZE		1024
/*********************/
/*  IEC Types defs   */
/*********************/

typedef uint8_t  IEC_BOOL;

typedef int8_t    IEC_SINT;
typedef int16_t   IEC_INT;
typedef int32_t   IEC_DINT;
typedef int64_t   IEC_LINT;

typedef uint8_t    IEC_USINT;
typedef uint16_t   IEC_UINT;
typedef uint32_t   IEC_UDINT;
typedef uint64_t   IEC_ULINT;

typedef uint8_t    IEC_BYTE;
typedef uint16_t   IEC_WORD;
typedef uint32_t   IEC_DWORD;
typedef uint64_t   IEC_LWORD;

typedef float    IEC_REAL;
typedef double   IEC_LREAL;

//Booleans
extern IEC_BOOL *bool_input[BUFFER_SIZE][8];
extern IEC_BOOL *bool_output[BUFFER_SIZE][8];

//Bytes
extern IEC_BYTE *byte_input[BUFFER_SIZE];
extern IEC_BYTE *byte_output[BUFFER_SIZE];

//Analog I/O
extern IEC_UINT *int_input[BUFFER_SIZE];
extern IEC_UINT *int_output[BUFFER_SIZE];

//Memory
extern IEC_UINT *int_memory[BUFFER_SIZE];
extern IEC_DINT *dint_memory[BUFFER_SIZE];
extern IEC_LINT *lint_memory[BUFFER_SIZE];

//Special Functions
extern IEC_LINT *special_functions[BUFFER_SIZE];

//Common task timer
extern unsigned long long common_ticktime__;

//----------------------------------------------------------------------
//FUNCTION PROTOTYPES
//----------------------------------------------------------------------

//MatIEC Compiler
void config_run__(unsigned long tick);
void config_init__(void);

//glueVars.c
void glueVars();
void updateTime();

//hardware_layer.c
void initializeHardware();
void finalizeHardware();
void updateBuffersIn();
void updateBuffersOut();
void updateModbusServerIn(modbusServerObj *obj);
void updateModbusServerOutWR(modbusServerObj *obj);
void updateModbusServerOutRD(modbusServerObj *obj);

//main.c
void delay_msec(struct timespec *ts, int delay);
bool pinNotPresent(int *ignored_vector, int vector_size, int pinNumber);
extern uint8_t run_openplc;
void handleSpecialFunctions();