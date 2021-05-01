#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <si/shunyaInterfaces.h>
#include <pthread.h>
#include "ladder.h"

#if !defined(ARRAY_SIZE)
#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#endif

#define MAX_INPUT 		14
#define MAX_OUTPUT 		11
#define MAX_ANALOG_OUT	1

/********************I/O PINS CONFIGURATION*********************
 * A good source for RaspberryPi I/O pins information is:
 * http://pinout.xyz
 *
 * The buffers below works as an internal mask, so that the
 * OpenPLC can access each pin sequentially
****************************************************************/
//inBufferPinMask: pin mask for each input, which
//means what pin is mapped to that OpenPLC input
//int inBufferPinMask[MAX_INPUT] = { 8, 9, 7, 0, 2, 3, 12, 13, 14, 21, 22, 23, 24, 25 };
int inBufferPinMask[MAX_INPUT] = { 3, 5, 7, 11, 13, 15, 19, 21, 23, 21, 29, 33, 35, 37 };

//outBufferPinMask: pin mask for each output, which
//means what pin is mapped to that OpenPLC output
//int outBufferPinMask[MAX_OUTPUT] =	{ 15, 16, 4, 5, 6, 10, 11, 26, 27, 28, 29 };
int outBufferPinMask[MAX_OUTPUT] =	{ 8, 10, 16, 18, 22, 24, 26, 32, 36, 38, 40 };

//analogOutBufferPinMask: pin mask for the analog PWM
//output of the RaspberryPi
//int analogOutBufferPinMask[MAX_ANALOG_OUT] = { 1 };
int analogOutBufferPinMask[MAX_ANALOG_OUT] = { 12 };

//-----------------------------------------------------------------------------
// This function is called by the main OpenPLC routine when it is initializing.
// Hardware initialization procedures should be here.
//-----------------------------------------------------------------------------
void initializeHardware()
{
    debugLogOn();
    initLib();

    //set pins as input
    for (int i = 0; i < MAX_INPUT; i++) {
        pinMode(inBufferPinMask[i], INPUT);
    }

    //set pins as output
    for (int i = 0; i < MAX_OUTPUT; i++) {
        pinMode(outBufferPinMask[i], OUTPUT);
    }
}

//-----------------------------------------------------------------------------
// This function is called by the main OpenPLC routine when it is finalizing.
// Resource clearing procedures should be here.
//-----------------------------------------------------------------------------
void finalizeHardware()
{
}

//-----------------------------------------------------------------------------
// This function is called by the OpenPLC in a loop. Here the internal buffers
// must be updated to reflect the actual state of the input pins. The mutex buffer_lock
// must be used to protect access to the buffers on a threaded environment.
//-----------------------------------------------------------------------------
void updateBuffersIn()
{
    //INPUT
    for (int i = 0; i < MAX_INPUT; i++) {
        if (bool_input[i/8][i%8] != NULL) {
            *bool_input[i/8][i%8] = digitalRead(inBufferPinMask[i]);
        }
    }
}

//-----------------------------------------------------------------------------
// This function is called by the OpenPLC in a loop. Here the internal buffers
// must be updated to reflect the actual state of the output pins. The mutex buffer_lock
// must be used to protect access to the buffers on a threaded environment.
//-----------------------------------------------------------------------------
void updateBuffersOut()
{
    //OUTPUT
    for (int i = 0; i < MAX_OUTPUT; i++) {
        if (bool_output[i/8][i%8] != NULL) {
            digitalWrite(outBufferPinMask[i], *bool_output[i/8][i%8]);
        }
    }
}

void updateModbusServerIn(modbusServerObj *obj)
{
    /* Update Modbus Input bits */
    for(int index = obj->mb_mapping->start_input_bits; index < obj->mb_mapping->nb_input_bits; index++)
    {
        if (bool_input[index/8][index%8] != NULL) {
            modbusSrvSetRegisters(obj, MODBUS_INPUT_COILS, index, *bool_input[index/8][index%8]);
        }        
    }

    /* Update Modbus Analog Input */
    for(int index = obj->mb_mapping->start_input_registers; index < obj->mb_mapping->nb_input_registers; index++)
    {
        if (bool_input[index/8][index%8] != NULL) {
            modbusSrvSetRegisters(obj, MODBUS_INPUT_REG, index, *int_input[index/8][index%8]);
        }        
    }

}

void updateModbusServerOutWR(modbusServerObj *obj)
{

    /* Update Modbus Output bits */
    for(int index = obj->mb_mapping->start_bits; index < obj->mb_mapping->nb_bits; index++)
    {
        if (bool_input[index/8][index%8] != NULL) {
            modbusSrvSetRegisters(obj, MODBUS_OUTPUT_COILS, index, *bool_output[index/8][index%8]);
        }        
    }

    /* Update Modbus Analog Output */
    for(int index = obj->mb_mapping->start_input_registers; index < obj->mb_mapping->nb_input_registers; index++)
    {
        if (bool_input[index/8][index%8] != NULL) {
            modbusSrvSetRegisters(obj, MODBUS_HOLDING_REG, index, *int_output[index/8][index%8]);
        }        
    }

}

void updateModbusServerOutRD(modbusServerObj *obj)
{

    /* Update Modbus Output bits */
    for(int index = obj->mb_mapping->start_bits; index < obj->mb_mapping->nb_bits; index++)
    {
        if (bool_input[index/8][index%8] != NULL) {
            *bool_output[index/8][index%8] = modbusSrvGetRegisters(obj, MODBUS_OUTPUT_COILS, index);
        }        
    }

    /* Update Modbus Analog Output */
    for(int index = obj->mb_mapping->start_input_registers; index < obj->mb_mapping->nb_input_registers; index++)
    {
        if (bool_input[index/8][index%8] != NULL) {
            *int_output[index/8][index%8] = modbusSrvGetRegisters(obj, MODBUS_HOLDING_REG, index);
        }        
    }

}
