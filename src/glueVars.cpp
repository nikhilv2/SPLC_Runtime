//-----------------------------------------------------------------------------
// Copyright 2015 Thiago Alves
// This file is part of the OpenPLC Software Stack.
//
// OpenPLC is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// OpenPLC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenPLC.  If not, see <http://www.gnu.org/licenses/>.
//------
//
// This file is responsible for gluing the variables from the IEC program to
// the OpenPLC memory pointers. It is automatically generated by the
// glue_generator program. PLEASE DON'T EDIT THIS FILE!
// Thiago Alves, May 2016
//-----------------------------------------------------------------------------

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


#define __LOCATED_VAR(type, name, ...) type __##name;
#include "LOCATED_VARIABLES.h"
#undef __LOCATED_VAR
#define __LOCATED_VAR(type, name, ...) type* name = &__##name;
#include "LOCATED_VARIABLES.h"
#undef __LOCATED_VAR

void glueVars()
{
	bool_input[0][0] = __IX0_0;
	bool_input[0][1] = __IX0_1;
	bool_output[0][0] = __QX0_0;
}

void updateTime()
{
	__CURRENT_TIME.tv_nsec += common_ticktime__;

	if (__CURRENT_TIME.tv_nsec >= 1000000000)
	{
		__CURRENT_TIME.tv_nsec -= 1000000000;
		__CURRENT_TIME.tv_sec += 1;
	}
}