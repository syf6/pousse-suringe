// Adapted from:
// https://github.com/mmurdoch/arduinounit/blob/master/examples/basic/basic.ino

#include <AUnit.h>

#include "Var.hpp"
#include "FSM.hpp"


test(fsm_installation){

  Var_SetVar_u8(VAR_ID_LED_OK, 0u );
  Var_SetVar_u8(VAR_ID_LED_WARNING, 0u );
  Var_SetVar_u8(VAR_ID_LED_ERROR, 0u );   
  Var_SetVar_u8(VAR_ID_SW_SYRING1,0u);
  Var_SetVar_u8(VAR_ID_BB_START,1u);
  Var_SetVar_u8(VAR_ID_BB_REV,0u);
  Var_GetVar_u8(VAR_ID_SW_ENDPOS1,0u);

  E_FSM e_returnState = FSM_Installation();

  assertEqual(e_returnState,FSM_INSTALLATION);
  assertEqual(Var_GetVar_u8( VAR_ID_LED_OK, last_value),1);
  assertEqual(Var_GetVar_u8( VAR_ID_LED_WARNING, last_value),0);
  assertEqual(Var_GetVar_u8( VAR_ID_LED_ERROR, last_value),0);
}

test(fsm_installation_failed){

  Var_SetVar_u8( VAR_ID_LED_OK, 0u );
  Var_SetVar_u8( VAR_ID_LED_WARNING, 0u );
  Var_SetVar_u8( VAR_ID_LED_ERROR, 0u );   
  Var_SetVar_u8(VAR_ID_SW_SYRING1,0u);
  Var_SetVar_u8(VAR_ID_BB_START,1u);
  Var_SetVar_u8(VAR_ID_BB_REV,0u);
  Var_GetVar_u8(VAR_ID_SW_ENDPOS1,0u);

  E_FSM e_returnState = FSM_Installation();

  assertEqual(e_returnState,FSM_INSTALLATION);
  assertEqual(Var_GetVar_u8( VAR_ID_LED_OK, last_value),0);
  assertEqual(Var_GetVar_u8( VAR_ID_LED_WARNING, last_value),1);
  assertEqual(Var_GetVar_u8( VAR_ID_LED_ERROR, last_value),0);
}

//----------------------------------------------------------------------------
// setup() and loop()
//----------------------------------------------------------------------------

void setup() {
#if ! defined(EPOXY_DUINO)
  delay(1000); // wait for stability on some boards to prevent garbage Serial
#endif
  Serial.begin(115200); // ESP8266 default of 74880 not supported on Linux
  while (!Serial); // for the Arduino Leonardo/Micro only
#if defined(EPOXY_DUINO)
  Serial.setLineModeUnix();
#endif

}

void loop() {
  aunit::TestRunner::run();
}
