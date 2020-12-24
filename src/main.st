PROGRAM Hello_World
  VAR
    my_button : BOOL;
    lamp : BOOL;
  END_VAR
  VAR
    TOF0 : TOF;
  END_VAR

  TOF0(IN := my_button, PT := T#2000ms);
  lamp := TOF0.Q;
END_PROGRAM


CONFIGURATION STD_CONF

  RESOURCE STD_RESSOURCE ON PLC
    TASK TaskMain(INTERVAL := T#50ms,PRIORITY := 0);
    PROGRAM Inst0 WITH TaskMain : Hello_World;
  END_RESOURCE
END_CONFIGURATION




