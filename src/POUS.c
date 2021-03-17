void PROGRAM0_init__(PROGRAM0 *data__, BOOL retain) {
  __INIT_LOCATED(BOOL,__IX1_4,data__->INP1,retain)
  __INIT_LOCATED_VALUE(data__->INP1,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__IX1_5,data__->INP2,retain)
  __INIT_LOCATED_VALUE(data__->INP2,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX1_2,data__->LED,retain)
  __INIT_LOCATED_VALUE(data__->LED,__BOOL_LITERAL(FALSE))
  __INIT_VAR(data__->AND4_OUT,__BOOL_LITERAL(FALSE),retain)
}

// Code part
void PROGRAM0_body__(PROGRAM0 *data__) {
  // Initialise TEMP variables

  __SET_VAR(data__->,AND4_OUT,,AND__BOOL__BOOL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (BOOL)__GET_LOCATED(data__->INP1,),
    (BOOL)__GET_LOCATED(data__->INP2,)));
  __SET_LOCATED(data__->,LED,,__GET_VAR(data__->AND4_OUT,));

  goto __end;

__end:
  return;
} // PROGRAM0_body__() 





