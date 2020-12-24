
#include "POUS.h"

void HELLO_WORLD_init__(HELLO_WORLD *data__, BOOL retain) {
  __INIT_VAR(data__->MY_BUTTON,__BOOL_LITERAL(FALSE),retain);
  __INIT_VAR(data__->LAMP,__BOOL_LITERAL(FALSE),retain);
  TOF_init__(&data__->TOF0,retain);
}

// Code part
void HELLO_WORLD_body__(HELLO_WORLD *data__) {
  // Initialise TEMP variables

  __SET_VAR(data__->TOF0.,IN,,__GET_VAR(data__->MY_BUTTON,));
  __SET_VAR(data__->TOF0.,PT,,__time_to_timespec(1, 2000, 0, 0, 0, 0));
  TOF_body__(&data__->TOF0);
  __SET_VAR(data__->,LAMP,,__GET_VAR(data__->TOF0.Q,));

  goto __end;

__end:
  return;
} // HELLO_WORLD_body__() 





