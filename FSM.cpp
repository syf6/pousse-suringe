/**
 * \file FSM.c
 * \author pierre.denis
 * \brief Finite state machine
**/

/*--------------------------------------------------------------------------*/
/* ... INCLUDES ...                                                         */
/*--------------------------------------------------------------------------*/

#include "HAL.hpp"
#include "Var.hpp"
#include "Task.hpp"
#include "FSM.hpp"
#include "REG.hpp"


/*--------------------------------------------------------------------------*/
/* ... DATAS ...                                                            */
/*--------------------------------------------------------------------------*/
E_FSM g_eFSM_State;
E_FSM e_state;          // Current State         
E_FSM e_previousState;  // Previous State
/*--------------------------------------------------------------------------*/
/* ... STATIC FUNC ...                                                      */
/*--------------------------------------------------------------------------*/

/**
 * @brief 
 * @details
**/
void FSM_DetectError( void )
{
  uint32_t u32CurrentError = Var_GetVar_u32(VAR_ID_ERROR,last_value);

  if(u32CurrentError != 0u)
  {
    g_eFSM_State = FSM_ERROR;
  }
  HAL_UpdateDisplayError(u32CurrentError);
}

/**
 * @brief Boot sequence (Turn On and Off all LEDs and go to Reset State)
 * @details 
 * @return New state to excecute
**/
E_FSM FSM_Boot( void )
{
  E_FSM returnState;
  returnState = FSM_IDLE;

  HAL_LedOK_TurnOn();
  HAL_LedError_TurnOn();
  HAL_LedWarning_TurnOn(); 
  delay(3000);
  HAL_LedOK_TurnOff(); 
  HAL_LedWarning_TurnOff();
  HAL_LedError_TurnOff();
  returnState = FSM_INSTALLATION;
  return( returnState );
}

/**
 * @brief Installation of the syringue phase
 * @details
 * @return New state to excecute
**/
E_FSM FSM_Installation( void )
{
  E_FSM returnState           = FSM_INSTALLATION;;       
  uint8_t u8_MicroSw_Syringe1 = Var_GetVar_u8( VAR_ID_SW_SYRING1,last_value);
  uint8_t u8_MicroSw_Reverse  = Var_GetVar_u8( VAR_ID_SW_REV,last_value);
  uint8_t u8_MicroSW_EndPos1  = Var_GetVar_u8( VAR_ID_SW_ENDPOS1,last_value);
  uint8_t u8_Button_Start     = Var_GetVar_u8( VAR_ID_BB_START,last_value);
  uint8_t u8_Button_Reverse   = Var_GetVar_u8( VAR_ID_BB_REV,last_value);
  uint8_t u8_SyringueActivated = Var_GetVar_u8( VAR_ID_PSH_SRG, last_value ); 
  uint32_t u32_CourantMotor = Var_GetVar_u32( VAR_ID_CUR_MOTOR,last_value );

  Var_ClearError();
  
  // To much current is consumed
  if(REG_analyzeCurrent(u32_CourantMotor))
  {
    HAL_Stop_Motor();
    Var_SetError( ERROR_OVERCURRENT );
    returnState = FSM_ERROR;
  }

  // Syringue has been placed
  if(u8_MicroSw_Syringe1 !=0)
  {
    HAL_Stop_Motor();
    if(!u8_MicroSW_EndPos1)
    {
      // Syringe is detected and start button pressed
      HAL_Stop_Motor();
      Var_SetVar_u8( VAR_ID_LED_OK, 1u );
      Var_SetVar_u8( VAR_ID_LED_WARNING, 0u );
      Var_SetVar_u8( VAR_ID_LED_ERROR, 0u );
      returnState = FSM_IDLE;
    }  
  }

  // Manual placement
  else
  {
    // Syringue not detected
    Var_SetVar_u8( VAR_ID_PSH_SRG, 0u ); 

    if(u8_Button_Start) /* Forward mvmt*/ 
    { 
      if(!u8_MicroSW_EndPos1)
      {
        // Forward Procedure (Button Start pressed)
        HAL_Relay_Seton();
        HAL_Forward_Motor(255);
        Var_SetVar_u8( VAR_ID_LED_OK, 1u );
        Var_SetVar_u8( VAR_ID_LED_WARNING, 0u );
        Var_SetVar_u8( VAR_ID_LED_ERROR, 0u );   
      }
      else
      {
        Var_SetVar_u8( VAR_ID_LED_WARNING, 1u );
        HAL_Stop_Motor();
        HAL_Relay_Seton();
      }
    }
    else if(u8_Button_Reverse) /* Backward mvmt*/ 
    { 
      if(!u8_MicroSw_Reverse)
      {
        // Backward procedure ((reversed pressed)
        HAL_Relay_Seton();
        HAL_Backward_Motor();
        Var_SetVar_u8( VAR_ID_LED_OK, 1u );
        Var_SetVar_u8( VAR_ID_LED_WARNING, 0u );
        Var_SetVar_u8( VAR_ID_LED_ERROR, 0u );   
      }
      else
      {
        HAL_Stop_Motor();
        HAL_Relay_Seton();
      }
    }
    else /* No mvmt asked*/
    {
      HAL_Stop_Motor();
      HAL_Relay_Seton();
    }
  } 
   
 return( returnState );
}

/**
 * @brief Wait for an event (button start pushed or microswitch syringe detected)
 * @details 
 *      => FSM_INSTALLATION 
 *      => FSM_WAIT
 * @return New state to execute
**/
E_FSM FSM_Idle( void )
{
  E_FSM returnState = FSM_IDLE;

  uint8_t u8_ButtonStart          = Var_GetVar_u8( VAR_ID_BB_START, last_value);
  uint8_t u8_OldButtonStart       = Var_GetVar_u8( VAR_ID_BB_START,previous_last);
  uint8_t u8_ButtonReverse        = Var_GetVar_u8( VAR_ID_BB_REV,last_value);
  uint8_t u8_MicroSw_Syringe1     = Var_GetVar_u8( VAR_ID_SW_SYRING1,last_value);

  if(!u8_MicroSw_Syringe1)
  {
    // Syringue is not detected anymore
    HAL_Stop_Motor();
    returnState = FSM_INSTALLATION;
  }
  else if((u8_ButtonStart) && (!u8_OldButtonStart) && (!u8_ButtonReverse) )
  {
    // Scan for rising edge on start and no push on reverse
    returnState = FSM_WAIT;
  }
 return( returnState );
}

/**
 * @brief Wait for button to be release to run
 * @details
 * @return New state to execute
**/
E_FSM FSM_Wait( void )
{
  E_FSM returnState = FSM_WAIT;
  uint8_t u8_ButtonStart = Var_GetVar_u8( VAR_ID_BB_START ,last_value);
  uint8_t u8_ButtonReverse = Var_GetVar_u8( VAR_ID_BB_REV   ,last_value);

  if( (!u8_ButtonStart) && (!u8_ButtonReverse) )
  {
    // If both buttons are released, Run
    returnState = FSM_RUN;
  }

  return( returnState );
}

/**
 * @brief Handle mvmt and safety reading
 * @details
 * @return New state to execute
**/
E_FSM FSM_Run( void )
{
  E_FSM returnState = FSM_RUN;
  
  uint8_t u8_PushSyringeOn  = Var_GetVar_u8( VAR_ID_PSH_SRG,last_value);
  uint8_t u8_SwSyringe1     = Var_GetVar_u8( VAR_ID_SW_SYRING1 ,last_value);
  uint8_t u8_SwEndPos1      = Var_GetVar_u8( VAR_ID_SW_ENDPOS1,last_value );
  uint8_t u8_ButtonStart    = Var_GetVar_u8( VAR_ID_BB_START,last_value);
  uint8_t u8_ButtonReverse  = Var_GetVar_u8( VAR_ID_BB_REV,last_value);
  uint8_t u8_PotLevel       = Var_GetVar_u8( VAR_ID_POT_LEVEL,last_value );
  uint32_t u32_TensionMotor = Var_GetVar_u32( VAR_ID_VOLT_SUPPLY,last_value );
  uint32_t u32_CourantMotor = Var_GetVar_u32( VAR_ID_CUR_MOTOR,last_value );

  /* Handle Error Case */
  bool b_overcurrent = REG_analyzeCurrent(u32_CourantMotor);
  if(!u8_SwSyringe1 || b_overcurrent)
  {
    HAL_Stop_Motor();
    // Syringue is no longer present
    if (!u8_SwSyringe1)
    {
      Var_SetError( ERROR_SYRINGE );
      returnState = FSM_REMOVE;
    }
    // To much current is consumed
    if(b_overcurrent)
    {
      Var_SetError( ERROR_OVERCURRENT );
      returnState = FSM_ERROR;
    }    
  }

  // Nominal case: move according to potentiometer
  switch(u8_PotLevel)
  {
      case 0:
        HAL_Forward_Motor(0);
        break;
      case 1:
        HAL_Forward_Motor(150);
        break;            
      case 2:
        HAL_Forward_Motor(200);
        break;
      case 3:
        HAL_Forward_Motor(255);
        break;          
  }
  
  // Reach end of journey
  if(u8_SwEndPos1)
  {
    HAL_Stop_Motor();
    Var_SetVar_u8( VAR_ID_LED_OK, 1u );
    Var_SetVar_u8( VAR_ID_LED_WARNING, 1u );
    returnState = FSM_REMOVE;
  }

  return( returnState );
}

/**
 * @brief Wait for the syringue to be removed
 * @details
 * @return New state to execute
**/
E_FSM FSM_RemoveDevice( void )
{
  E_FSM returnState = FSM_REMOVE;
  uint8_t u8_SwSyringe1 = Var_GetVar_u8( VAR_ID_SW_SYRING1,last_value);
 
  if(!u8_SwSyringe1)
  {
    Var_SetVar_u8( VAR_ID_LED_WARNING, 0u );
    returnState = FSM_INSTALLATION;
  }
 return( returnState );
}

/**
 * @brief Error State
 * @return New state to execute
**/
E_FSM FSM_Error( void )
{
  E_FSM returnState;
  returnState = FSM_ERROR;
  
  Var_SetVar_u8( VAR_ID_PSH_SRG, 0u ); 
  Var_SetVar_u8( VAR_ID_LED_OK, 0u );
  Var_SetVar_u8( VAR_ID_LED_WARNING, 0u );
  Var_SetVar_u8( VAR_ID_LED_ERROR, 1u );

 return( returnState );
}

/*--------------------------------------------------------------------------*/
/* ... EXT FUNC  IMP ...                                                    */
/*--------------------------------------------------------------------------*/

/**
 * @brief Reset static variable to a known state.
 * @details
**/
void FSM_Init( void )
{
  g_eFSM_State = FSM_BOOT;
  Var_SetVar_u8(VAR_ID_PSH_SRG, 0);
}

/**
 * @brief Manage finite state machine (state action & transition)
 * @details
**/
void FSM_Handler( void )
{
  FSM_DetectError();
  
  e_previousState = e_state;

  switch( e_previousState )
  {
      default :
      case FSM_BOOT : 
        e_state = FSM_Boot();
        break;
      case FSM_INSTALLATION :
        e_state = FSM_Installation();
        break;   
      case FSM_IDLE :
        e_state = FSM_Idle();
        break;
      case FSM_WAIT :
        e_state = FSM_Wait();
        break;
      case FSM_RUN :
        e_state = FSM_Run();
        break;
      case FSM_REMOVE :
        e_state = FSM_RemoveDevice();
        break;
      case FSM_ERROR :
        e_state = FSM_Error();
        break;
  }
  
  return(e_state);
}

/**
 * @brief Recover current state
 * @details
**/
E_FSM FSM_GetCurrentState(void)
{
  return e_state;
}

/**
 * @brief Recover previous state
 * @details
**/
E_FSM FSM_GetPreviousState(void)
{
  return e_previousState;
}