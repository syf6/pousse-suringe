/**
 * \file Debug.h
 * \author pierre.denis
 * \brief Finite state machine
**/

#ifndef _DEBUG_H_
#define _DEBUG_H_

/*--------------------------------------------------------------------------*/
/* ... INCLUDES ...                                                         */
/*--------------------------------------------------------------------------*/
#include <Arduino.h>
#include <string.h>

#include "FSM.hpp"
#include "Var.hpp"
#include "HAL.hpp"

const String ga_u8DispVar32[VAR_ID_32_MAX] =
  {
    String("Error:"), /*!< VAR_ID_ERROR = 0, */
    String("Voltage:"), /*!< VAR_ID_VOLT_SUPPLY, */
    String("Current:"), /*!< VAR_ID_CUR_MOTOR, */  
    String("Potentiometer:"), /*!< VAR_ID_POS_SELECT, */
  };

const String ga_u8DispVar8[VAR_ID_08_MAX] =
{
  String("Led OK :"),                 /*!< VAR_ID_LED_OK  */
  String("Led Error :"),              /*!< VAR_ID_LED_ERROR  */
  String("Led Xarning :"),            /*!< VAR_ID_LED_WARNING  */
  String("Microswitch PS1:"),         /*!< VAR_ID_SW_SYRING1  */
  String("Microswitch PS2:"),         /*!< VAR_ID_SW_SYRING2 */
  String("Microswitch AR:"),          /*!< VAR_ID_SW_REV */
  String("Microswitch AV1:"),         /*!< VAR_ID_SW_SYRING1  */
  String("Microswitch AV2:"),         /*!< VAR_ID_SW_SYRING2 */
  String("Button Start:"),            /*!< VAR_ID_BB_START */
  String("Button Reverse:"),          /*!< VAR_ID_BB_REV */
  String("Potentiometer Level:"),     /*!< POTENTIOMETER */
  String("Syringue Used :"),          /*!< VAR_ID_PSH_SRG */
  String("CapteurMvmt1:"),            /*!< VAR_ID_CAP_MVM_1 */
  String("CapteurMvmt2:")             /*!< VAR_ID_CAP_MVM_2 */
};


/**
 * @brief Debug Monitoring Variables 
 * @details
**/
void DebugMonitoring()
{  
  uint32_t u32_old = Var_GetVar_u32(VAR_ID_VOLT_SUPPLY,previous_last);
  uint32_t u32_new = Var_GetVar_u32(VAR_ID_VOLT_SUPPLY,last_value);

  if(abs(u32_old-u32_new)>10)
  {
    Serial.println(ga_u8DispVar32[VAR_ID_VOLT_SUPPLY]+Var_GetVar_u32(VAR_ID_VOLT_SUPPLY,last_value));
  }

  u32_old = Var_GetVar_u32(VAR_ID_CUR_MOTOR,previous_last);
  u32_new = Var_GetVar_u32(VAR_ID_CUR_MOTOR,last_value);
  if(abs(u32_old-u32_new)>2)
  {
    Serial.println(ga_u8DispVar32[VAR_ID_CUR_MOTOR]+Var_GetVar_u32(VAR_ID_CUR_MOTOR,last_value));
  }

  for (uint8_t u8_i = VAR_ID_CAP_MVM_1; u8_i <= VAR_ID_CAP_MVM_2;u8_i++)
  {
    if(Var_GetVar_u8(u8_i,last_value) != Var_GetVar_u8(u8_i,previous_last))
    {
      Serial.println(ga_u8DispVar8[u8_i]+Var_GetVar_u8(u8_i,last_value));
    }
  }

}

/**
 * @brief Serial Monitor Buttons
 * @details
**/
void DebugButtons() 
{
  uint8_t oldValue;
  uint8_t newValue;
  for (uint8_t u8_i = VAR_ID_BB_START; u8_i <= VAR_ID_POT_LEVEL;u8_i++)
  {
    newValue = Var_GetVar_u8(u8_i,last_value);
    oldValue = Var_GetVar_u8(u8_i,previous_last);
    if(newValue != oldValue)
    {
      Serial.println(ga_u8DispVar8[u8_i]+newValue);
    }
  }
}

/**
 * @brief Serial Monitor Microswitches
 * @details
**/
void DebugMicroswitches() 
{
  uint8_t oldValue;
  uint8_t newValue;
  for (uint8_t u8_i = VAR_ID_SW_SYRING1; u8_i <= VAR_ID_SW_ENDPOS2;u8_i++)
  {
    newValue = Var_GetVar_u8(u8_i,last_value);
    oldValue = Var_GetVar_u8(u8_i,previous_last);
    if(newValue != oldValue)
    {
      Serial.println(ga_u8DispVar8[u8_i]+newValue);
    }
  }
}

/**
 * @brief Serial Monitor Display state
 * @details
**/
void DebugState( void ) 
{

    if(FSM_GetCurrentState() != FSM_GetPreviousState())
    {
      Serial.flush();
      Serial.print( "FSMState:\t");
      switch(FSM_GetCurrentState())
      {
        case FSM_IDLE :
            Serial.println("IDLE");
            break;
        case FSM_INSTALLATION:
            Serial.println("INSTALLATION");
            break;
        case FSM_WAIT:
            Serial.println("START");
            break;   
        case FSM_RUN:
            Serial.println("RUN");
            break;
        case FSM_REMOVE:
            Serial.println("REMOVE");
            break;
        case FSM_ERROR:
            Serial.println("ERROR");
            break;
        default :
        case FSM_STATE_MAX:
            Serial.println("UNKNOW");
            break;
      }
    }

}


#endif
