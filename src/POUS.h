#ifndef __POUS_H
#define __POUS_H

#include "accessor.h"
#include "iec_std_lib.h"



// PROGRAM HELLO_WORLD
// Data part
typedef struct {
  // PROGRAM Interface - IN, OUT, IN_OUT variables

  // PROGRAM private variables - TEMP, private and located variables
  __DECLARE_VAR(BOOL,MY_BUTTON)
  __DECLARE_VAR(BOOL,LAMP)
  TOF TOF0;

} HELLO_WORLD;

void HELLO_WORLD_init__(HELLO_WORLD *data__, BOOL retain);
// Code part
void HELLO_WORLD_body__(HELLO_WORLD *data__);
#endif //__POUS_H



