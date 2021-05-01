/**
 * @file main.c
 * @brief PLC runtime for shunya OS.
 * 
 */
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#include "iec_types.h"
#include "ladder.h"
#include "log.h"

#include <si/shunyaInterfaces.h>
#include <si/modbus_server.h>
#include <signal.h>

#define OPLC_CYCLE          50000000

extern int opterr;
//extern int common_ticktime__;
IEC_BOOL __DEBUG;

IEC_LINT cycle_counter = 0;

unsigned long __tick = 0;
pthread_mutex_t bufferLock; //mutex for the internal buffers
uint8_t run_openplc = 1; //Variable to control OpenPLC Runtime execution

static modbusServerObj modbusServer;

//-----------------------------------------------------------------------------
// Helper function - 
//-----------------------------------------------------------------------------

/**
 * @brief Sleep for the ammount of time in milliseconds
 * 
 * @param ts timer
 * @param delay delay in milliseconds
 */
void delay_msec(struct timespec *ts, int delay)
{
    ts->tv_nsec += delay;
    if(ts->tv_nsec >= 1000*1000*1000)
    {
        ts->tv_nsec -= 1000*1000*1000;
        ts->tv_sec++;
    }
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, ts,  NULL);
}

//-----------------------------------------------------------------------------
// Verify if pin is present in one of the ignored vectors
//-----------------------------------------------------------------------------
bool pinNotPresent(int *ignored_vector, int vector_size, int pinNumber)
{
    for (int i = 0; i < vector_size; i++)
    {
        if (ignored_vector[i] == pinNumber)
            return false;
    }
    
    return true;
}

//-----------------------------------------------------------------------------
// Disable all outputs
//-----------------------------------------------------------------------------
void disableOutputs()
{
    //Disable digital outputs
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (bool_output[i][j] != NULL) *bool_output[i][j] = 0;
        }
    }
    
    //Disable byte outputs
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        if (byte_output[i] != NULL) *byte_output[i] = 0;
    }
    
    //Disable analog outputs
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        if (int_output[i] != NULL) *int_output[i] = 0;
    }
}

void initializeModbusServer(modbusServerObj *obj)
{
    int rc;
    /* Set Modbus server Settings */
    modbusServer.set.isSettingVerified = true;
    modbusServer.set.type = SI_MODBUS_SRV_TYPE_TCP;
    modbusServer.set.slaveAddress = 9;
    modbusServer.set.port = 9595;
    modbusServer.set.device[0] = '\0';
    modbusServer.set.baudrate = 9600;
    /* Modbus Server Register Map settings */
    modbusServer.set.startAddrOutputCoil = 0;
    modbusServer.set.numOutputCoilReg = 800;
    modbusServer.set.startAddrInputCoil = 10000; 
    modbusServer.set.numInputCoilReg = 800;
    modbusServer.set.startAddrInputReg = 30000;      
    modbusServer.set.numInputReg = 1024;
    modbusServer.set.startAddrHoldingReg = 40000;  
    modbusServer.set.numHoldingReg = 2048;

    log_info("Initializing the Modbus Server");
    /* Initialize the Modbus Slave/Server*/
    rc = modbusSrvInit(&modbusServer);
    if (rc < 0) {
        log_error("Failed to initialize the Modbus Server/Slave");
    } else {
        log_info("Init success\n");
    }

    log_info("Starting the Modbus Server");
    rc = modbusSrvStart(&modbusServer);
    if (rc < 0) {
        log_error("Failed to start the Modbus Server/Slave");
    } else {
        log_info("Start success\n");
    }
}

//-----------------------------------------------------------------------------
// Special Functions
//-----------------------------------------------------------------------------
void handleSpecialFunctions()
{
    //current time [%ML1024]
    struct tm *current_time;
    time_t rawtime;
    
    time(&rawtime);
    // store the UTC clock in [%ML1027]
    if (special_functions[3] != NULL) *special_functions[3] = rawtime;

    current_time = localtime(&rawtime);
    
    rawtime = rawtime - timezone;
    if (current_time->tm_isdst > 0) rawtime = rawtime + 3600;
        
    if (special_functions[0] != NULL) *special_functions[0] = rawtime;
    
    //number of cycles [%ML1025]
    cycle_counter++;
    if (special_functions[1] != NULL) *special_functions[1] = cycle_counter;
    
}

// Define the function to be called when ctrl-c (SIGINT) signal is sent to process
void signal_callback_handler(int signum)
{
    int rc = -1;
    
    log_trace("Caught signal %d\n",signum);

    /* --- Cleanup and close up stuff here --- */

    /* ------ Modbus Server Cleanup ------- */
    /* Stop Modbus server */
    log_info("Stopping Modbus server\n");
    rc = modbusSrvStop(&modbusServer);
    if (rc < 0) {
        log_warn("Failed to stop Modbus Server\n");
    } else {
        log_info("Stop success\n");
    }
    /* Destroy the Modbus server */
    log_info("Destroying Modbus server  variables and memory\n");
    rc = modbusSrvDestroy(&modbusServer);
    if (rc < 0) {
        log_warn("Failed to Destroy Modbus Server\n");
    } else {
        log_info("Destroy success\n");
    }

    log_info("Disabling outputs\n");
    disableOutputs();
    //updateCustomOut();
    updateBuffersOut();
	finalizeHardware();
    log_info("Shutting down Runtime...\n");
    // Terminate program
    exit(signum);
}

int main(int argc,char **argv)
{
    signal(SIGINT, signal_callback_handler);
    /* Start Logging */
    log_set_level(LOG_TRACE);

    /* Set log to quite mode i.e does not print to stderr */
    //log_set_quiet(true);

    /*======================================================
     *                 PLC INITIALIZATION
     *======================================================
     */
    time_t start_time;
    tzset();
    time(&start_time);
    config_init__();

    //======================================================
    //              HARDWARE INITIALIZATION
    //======================================================
    initializeHardware();
    initializeModbusServer(&modbusServer);
    updateBuffersIn();
    updateBuffersOut();

    //======================================================
    //              REAL-TIME INITIALIZATION
    //======================================================
    // Set our thread to real time priority
    struct sched_param sp;
    sp.sched_priority = 30;
    log_info("Setting main thread priority to RT\n");
    if(pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp))
    {
        log_warn("Failed to set main thread to real-time priority\n");
    }

    // Lock memory to ensure no swapping is done.
    log_info("Locking main thread memory\n");
    if(mlockall(MCL_FUTURE|MCL_CURRENT))
    {
        log_warn("Failed to lock memory\n");
    }

	//gets the starting point for the clock
	log_info("Getting current time\n");
	struct timespec timer_start;
	clock_gettime(CLOCK_MONOTONIC, &timer_start);

	//======================================================
	//                    MAIN LOOP
	//======================================================
    log_info("Running PLC runtime ..");
	while(run_openplc)
	{
		//make sure the buffer pointers are correct and
		//attached to the user variables
		glueVars();
        
		updateBuffersIn(); //read input image

        updateModbusServerIn(&modbusServer);

        handleSpecialFunctions();

		config_run__(__tick++); // execute plc program logic

        updateModbusServerOutRD(&modbusServer);
		
        updateBuffersOut(); //write output image
        
        updateModbusServerOutWR(&modbusServer);
		
        updateTime();

		delay_msec(&timer_start, common_ticktime__);
	}
    
    //======================================================
	//             SHUTTING DOWN OPENPLC RUNTIME
	//======================================================
    //pthread_join(interactive_thread, NULL);
    log_info("Disabling outputs\n");
    disableOutputs();
    //updateCustomOut();
    updateBuffersOut();
	finalizeHardware();
    log_info("Shutting down Runtime...\n");
    exit(0);
}
