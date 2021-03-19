PROGRAM program0
  VAR
    LocalVar0 AT %IX0.1 : BOOL;
    LocalVar1 AT %QX0.0 : BOOL;
  END_VAR
  VAR
    LocalVar2 : INT := 55;
    LocalVar3 : INT := 45;
    GT3_ENO : BOOL;
    GT3_OUT : BOOL;
  END_VAR

  GT3_OUT := GT(EN := LocalVar0, IN1 := LocalVar2, IN2 := LocalVar3, ENO => GT3_ENO);
  LocalVar1 := GT3_OUT;
END_PROGRAM


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    TASK task0(INTERVAL := T#20ms,PRIORITY := 0);
    PROGRAM instance0 WITH task0 : program0;
  END_RESOURCE
END_CONFIGURATION
