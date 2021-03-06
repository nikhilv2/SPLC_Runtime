#include "iec_std_lib.h"

TIME __CURRENT_TIME;
extern unsigned long long common_ticktime__;

/* PLC Buffer size */
#define PLC_BUFFER_SIZE	1024
#define PLC_WORD_SIZE 8
/* Booleans */
IEC_BOOL *boolInputs[PLC_BUFFER_SIZE][PLC_WORD_SIZE];
IEC_BOOL *boolOutputs[PLC_BUFFER_SIZE][PLC_WORD_SIZE];

/* Bytes */
IEC_BYTE *byteInputs[PLC_BUFFER_SIZE];
IEC_BYTE *byteOutputs[PLC_BUFFER_SIZE];

/* Analog I/O */
IEC_UINT *uintInputs[PLC_BUFFER_SIZE];
IEC_UINT *uintOutputs[PLC_BUFFER_SIZE];

/* Memory */
IEC_UINT *intMemory[PLC_BUFFER_SIZE];
IEC_DINT *dintMemory[PLC_BUFFER_SIZE];
IEC_LINT *lintMemory[PLC_BUFFER_SIZE];


#define __LOCATED_VAR(type, name, ...) type __##name;
#include "LOCATED_VARIABLES.h"
#undef __LOCATED_VAR
#define __LOCATED_VAR(type, name, ...) type* name = &__##name;
#include "LOCATED_VARIABLES.h"
#undef __LOCATED_VAR

extern void updateTime();

extern void initDS();