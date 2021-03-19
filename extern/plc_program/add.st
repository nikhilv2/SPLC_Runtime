PROGRAM program0
  VAR
    inp1 AT %QW0.0 : DINT;
    reg1 AT %QW0.1 : DINT;
    inp2 AT %QW0.2 : DINT;
    reg2 AT %QW0.3 : DINT;
    out AT %QW0.4 : DINT;
    out_reg AT %QW0.5 : DINT;
  END_VAR
  VAR
    ADD6_OUT : DINT;
  END_VAR

  ADD6_OUT := ADD(reg1, reg2);
  out_reg := ADD6_OUT;
END_PROGRAM


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    TASK task0(INTERVAL := T#20ms,PRIORITY := 0);
    PROGRAM instance0 WITH task0 : program0;
  END_RESOURCE
END_CONFIGURATION
