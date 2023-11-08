/**
 * \file FSM.c
 * \author pierre.denis
 * \brief Hardware Abstraction Layer
**/

/**\page HAL_Page Hardware Abstractive Layer Class
 * \section sectionDescription_HAL Description
 * HAL  class allow task event management and launch appropriate phase with global variable ge_FSM_Phase
 * This class consists of the following files :
 * - HAL.c
*/

/*--------------------------------------------------------------------------*/
/* ... INCLUDES ...                                                         */
/*--------------------------------------------------------------------------*/

#include "HAL.hpp"
#include "LLD.hpp"
#include "Var.hpp"
#include <SoftwareSerial.h>

/*--------------------------------------------------------------------------*/
/* ... DATAS ...                                                            */
/*--------------------------------------------------------------------------*/
extern uint32_t periode1;
extern uint32_t periode2;

/*--------------------------------------------------------------------------*/
/* ... FUNCTIONS ...                                                        */
/*--------------------------------------------------------------------------*/

/*! Software Serial Library Init (8,7 if problem)*/
const byte txPin = 5; // transmit pin 
const byte rxPin = 7; // receive pin 
SoftwareSerial s7s(rxPin, txPin);


/**
 * \fn void HAL_Init( void )
 * \brief 
 * \details
**/
void HAL_Init(void)
{
  LLD_PinInit();
  HAL_LedOK_TurnOff(); 
  HAL_LedWarning_TurnOff();
  HAL_LedError_TurnOff();
  HAL_Relay_Setoff();

  Var_SetVar_u8( VAR_ID_BB_START, HAL_ButtonStart_Get());
  Var_SetVar_u8( VAR_ID_BB_REV, HAL_ButtonReverse_Get());
  Var_SetVar_u8( VAR_ID_POT_LEVEL, HAL_Potentiometre());
  Var_SetVar_u8(VAR_ID_SW_SYRING1,HAL_Switch_PS1_Get());
  // Var_SetVar_u8(VAR_ID_SW_SYRING2,HAL_Switch_PS2_Get());
  Var_SetVar_u8(VAR_ID_SW_REV,HAL_Switch_AR_Get());
  Var_SetVar_u8(VAR_ID_SW_ENDPOS1,HAL_Switch_AV1_Get());
  Var_SetVar_u8(VAR_ID_SW_ENDPOS2,HAL_Switch_AV2_Get());
  Var_SetVar_u32( VAR_ID_VOLT_SUPPLY, HAL_Mesure_Tension_alim() );
  Var_SetVar_u32( VAR_ID_CUR_MOTOR, HAL_Mesure_Courant_motor() ); 

  s7s.begin(9600);
  s7s.write(0x76); // clear Display
  HAL_UpdateDisplay(Var_GetVar_u8( VAR_ID_POT_LEVEL,last_value));
}

/*! @addtogroup HAL Led group
 * @{
 */

/**
 * \fn void HAL_LedOK_TurnOn( void )
 * \brief 
 * \details
**/
void HAL_LedOK_TurnOn( void ){
  digitalWrite(4, HIGH);
} 

/**
 * \fn void HAL_LedOK_TurnOff( void )
 * \brief 
 * \details
**/
void HAL_LedOK_TurnOff( void ){
  digitalWrite(4, LOW);
} 

/**
 * \fn void HAL_LedWarning_TurnOn( void )
 * \brief 
 * \details
**/
void HAL_LedWarning_TurnOn( void ){
  digitalWrite(10, HIGH);
} 

/**
 * \fn void HAL_LedError_TurnOn( void )
 * \brief 
 * \details
**/
void HAL_LedWarning_TurnOff( void ){
  digitalWrite(10, LOW);
} 

/**
 * \fn void HAL_LedError_TurnOn( void )
 * \brief 
 * \details
**/
void HAL_LedError_TurnOn( void ){
  digitalWrite(9, HIGH);
} 

/**
 * \fn void HAL_LedError_TurnOff( void )
 * \brief 
 * \details
**/
void HAL_LedError_TurnOff( void ){
  digitalWrite(9, LOW);
} 

/**
 * @brief Update 7 bar display  
 * @details Func will write value in this order
  * 0x7B: Dig 1 control
  * Val:  Dig 1 value
  * 0x7C: Dig 2 control
  * Val:  Dig 2 value
 * @param u8PosPotard go from 0 -> 3
**/
void HAL_UpdateDisplay( uint8_t u8PosPotard ) 
{

  s7s.write(0x76); // clear Display
  switch( u8PosPotard )
  {
    case 0:
      s7s.write(0x7B);      
      s7s.write(0b1011110); 
      s7s.write(0x7C);      
      s7s.write(0b0111111); 
      break;
    case 1:
      s7s.write(0x7B);
      s7s.write(0b1011110);
      s7s.write(0x7C);
      s7s.write(0b0000110);
      break;
    case 2:
      s7s.write(0x7B);
      s7s.write(0b1011110);
      s7s.write(0x7C);
      s7s.write(0b1011011);
      break;
    case 3:
      s7s.write(0x7B);
      s7s.write(0b1011110);
      s7s.write(0x7C);
      s7s.write(0b1001111);
      break;
  }
}

/**
 * @brief Update 7 bar display  
 * @details Func will write value in this order: 
    0   1   2   3   4   5   6
    M   LU  LD  D   RD  RU  U
 * @param u8PosPotard go from 0 -> 3
**/
void HAL_UpdateDisplayError( uint8_t u8_error) 
{
  static uint8_t u8_buffer=ERROR_NO_ERROR;

  if (u8_error != u8_buffer)
  {
    u8_buffer = u8_error;

    switch(u8_error)
      {
        case ERROR_STOPBUTTON: // 'ES' Display
          s7s.write(0x76); 
          s7s.write(0x7B);      
          s7s.write(0b1111001); 
          s7s.write(0x7C);      
          s7s.write(0b1101101); 
          break;
        case ERROR_OVERCURRENT: // 'CUR' Display
          s7s.write(0x76); 
          s7s.write(0x7B);
          s7s.write(0b0111001);
          s7s.write(0x7C);
          s7s.write(0b0111110);
          s7s.write(0x7D);
          s7s.write(0b1110111);
          break;
      }
  }

  
}

/*! @} */

/*! @addtogroup HAL Buttons/Potentiometer group
 * @{
 */


/** @brief Scan if Start button pressed (DIO6)
 * @details
 * @return 1 if pressed / 0 otherwise
**/
uint8_t HAL_ButtonStart_Get( void ){
  uint8_t result = 0;
  if (digitalRead(6) == 0)
    result = 1;  
  return result;  
} 

/** @brief Scan if Reverse button pressed (DIO12)
 * @details
 * @return 1 if pressed / 0 otherwise
**/
uint8_t HAL_ButtonReverse_Get( void ){
  uint8_t result = 0;
  if (digitalRead(12) == 0)
    result = 1;  
  return result;   
} 

/** @brief  Scan if Potentiometer Value (A2)
 * @details
 *    val = map(val, 0, 1023, -15, 165);  // si on règle la vitesse avec le bouton
 *    val = map(val, 0, 1023, 0, 20);     // si on règle le volume à injecter avec le bouton
 *    val = map(val, 0, 1023, -1, 11);    // si on règle le mode de débit avec le bouton...
**/
uint8_t HAL_Potentiometre( void )
{
  uint32_t u32_val = analogRead(A2);
  uint8_t u8_returnValue;
  
  Var_SetVar_u32(VAR_ID_POT_RAW,u32_val);

  if( (255 <= u32_val) && (u32_val < 511) )
  { 
    u8_returnValue = 1; 
  }
  else if( (512 <= u32_val) && (u32_val < 766) )
  { 
    u8_returnValue = 2; 
  }
  else if( u32_val >= 767 )
  { 
    u8_returnValue = 3; 
  }
  else
  {
    u8_returnValue = 0; 
  }

  return( u8_returnValue );
}

/*! @} */

/*! @addtogroup HAL Switch monitoring group
 * @{
 */

/** @brief Scan if Syringue is detected
 * @details Input Pull Up
 * @return True if switch detect a syringue / False in other case
 * 
**/
uint8_t HAL_Switch_PS1_Get( void )
{
  if( analogRead(A5) > 30 )
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

/** @brief Limit Switch 1 front reached scan
 * @details
 * @return True if switch detect front limit / False in other case
**/
uint8_t HAL_Switch_PS2_Get( void )
{
  if( digitalRead(1))
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

/** @brief Detect if rear switch has been detected
 * @details
 * @return  True if switch detect back limit / False in other case
**/
uint8_t HAL_Switch_AR_Get( void )
{  
  if( analogRead(A4) > 30 )
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

/** @brief Scan if Syringue is detected (A1)
 * @details Input Pull Up
 * @return True if switch detect a syringue / False in other case
**/
uint8_t HAL_Switch_AV1_Get( void ) {
  if (analogRead(A1) > 30)
    return 0;
  else
    return 1;
}

/** @brief Limit Switch 2 front reached scan
 * @details
 * @return True if switch detect front limit / False in other case
**/
uint8_t HAL_Switch_AV2_Get( void )
{
  if (digitalRead(0))
    return 0;
  else
    return 1;
}

/**
 * \brief 
 * \details
**/
uint8_t HAL_CaptMov1_Get(void)
{
  if (digitalRead(2))
    return 0;
  else
    return 1;
}

/**
 * \brief 
 * \details
**/
uint8_t HAL_CaptMov2_Get(void)
{
  if (digitalRead(3))
    return 0;
  else
    return 1;
}

/*! @} */


/*! @addtogroup HAL Motor control group
 * @{
 */

/**
 * \fn void HAL_Relay_SetOn( void )
 * \brief Turn on static relay
 * \details
**/
void HAL_Relay_Seton( void )
{
  digitalWrite(7, HIGH);
} 

/**
 * \fn void HAL_Relay_Setoff( void )
 * \brief Turn off static relay
 * \details
**/
void HAL_Relay_Setoff( void )
{
  digitalWrite(7, LOW);
}

/**
 * @fn void HAL_Forward_Motor( uint32_t u32Speed )
 * @brief Order fwd mvmt 
 * @details
 * @param u32Speed speed in forward direction 
**/
void HAL_Forward_Motor( uint32_t u32Speed )
{
  digitalWrite(11, LOW);        // Direction  moteur B AVANCE PISTON
  digitalWrite(8, LOW);         // Frein  moteur B
  analogWrite(13, u32Speed );   // PWM  moteur B, avec régulation de vitesse ou de débit
} 

/**
 * @fn void HAL_Forward_Motor( uint32_t u32Speed )
 * @brief Order backward mvmt 
 * @details
 * @param u32Speed speed in forward direction 
**/
void HAL_Backward_Motor( void )
{
  digitalWrite(11, HIGH); // Direction moteur B AVANCE PISTON
  digitalWrite(8, LOW);   // Frein  moteur B
  analogWrite(13, 500);   // PWM  moteur B
} 

/**
 * \fn void HAL_Stop_Motor( void )
 * \brief 
 * \details
**/
void HAL_Stop_Motor( void )
{
  digitalWrite(8, HIGH);   // Frein  moteur B
  analogWrite(13, 0);      // PWM  moteur B
} 

/**
 * @fn uint32_t HAL_Mesure_Vitesse1( void )
 * @brief Recover speed from motor (sens1)
 * @details ((1000000/ (3 x 2 period))*60)/52.734
 * @return speed from first sensor
**/
uint32_t HAL_Mesure_Vitesse1( void )
{
  // TODO: measurement
  uint32_t vitesse1;
  vitesse1 =  189631 / (uint32_t)(periode1) ;  
  return (vitesse1);  
} 

/**
 * @fn uint32_t HAL_Mesure_Vitesse2( void )
 * @brief Recover speed from motor (sens2)
 * @details ((1000000/ (3 x 2 period))*60)/52.734
 * @return speed from first sensor
**/
uint32_t HAL_Mesure_Vitesse2( void )
{
  // TODO: measurement
  uint32_t vitesse2 =  189631 / (uint32_t)(periode2) ;  
  return (vitesse2);  
} 

/**
 * @fn uint32_t HAL_Mesure_Tension_alim( void )
 * @brief Recover voltage from power supply 
 * @details (5V x pont x (4/3))/1024  1kohm et 3kohm
**/
uint32_t HAL_Mesure_Tension_alim( void )
{
  uint32_t u32_Pont = analogRead(A0);
  uint32_t u32_Tension = ( (20 * u32_Pont) / 307 ); 
  return( u32_Tension );
}

/**
 * @brief Recover current consumed
 * @return current : in mA
**/
uint32_t HAL_Mesure_Courant_motor( void )
{
  int adc  = analogRead(A3);
  uint32_t u32_current = map(adc, 0, 255, 0, 2000);
  return( u32_current);
}

/*! @} */
