/**
 * \file task.h
 * \author pierre.denis
 * \brief Task Scheduler simple
**/

/** \page TASK_Page Cyclic Task  Class
 * \section sectionDescription_TASK Description
 * TASK class allow task event management and launch appropriate phase with global variable ge_FSM_Phase
 * This class consists of the following files :
 * - TASK.c
 * \section sectionFunctions_TASK Function Description
 * \subsection subsectionInitFunctions Init Functions
 *  - TASK_Init()                    : Initialize static variables
 *
 * \subsection subsectionListFunctions_TASK List Functions
 * List of setter functions
 *  - Var_SetError() 
 *  - TASK_ClearEvent()
 *  - cyclicTask_HandleError() 
 *  - cyclicTask_UpdateHmi() 
 *  - cyclicTask_Display()
 *  - cyclicTask_Motor()
 *  - cyclicTask_GetMicroSwitch()
 *  - cyclicTask_GetHmiEvent()
 *  - cyclicTask_GetMonitorMotor()
 *  - TASK_ManageEvent()
 *  - TASK_ToggleEvent()

*/

/*--------------------------------------------------------------------------*/
/* ... INCLUDES ...                                                         */
/*--------------------------------------------------------------------------*/

#include "Task.hpp"
#include "HAL.hpp"
#include "Var.hpp"

#define MASK_TASK_MONITOR_MOTOR       0x0001u
#define MASK_TASK_MICRO_SW            0x0002u
#define MASK_TASK_VOLTAGE             0x0004u
#define MASK_TASK_GET_HMI             0x0008u
#define MASK_TASK_UPDATE_HMI          0x0010u
#define MASK_TASK_LED                 0x0020u
#define MASK_TASK_ERROR               0x0040u
#define MASK_ALL                      0xFFFFu

uint16_t gu16_EventTask;

/*--------------------------------------------------------------------------*/
/* ... LOC FUNCTIONS ...                                                    */
/*--------------------------------------------------------------------------*/

/**
 * @brief Clear event when done
 * @details
 * @param u32_Mask mask corresponding to the event to ack
**/
void TASK_ClearEvent( uint16_t u16_Mask )
{
  gu16_EventTask &= ~u16_Mask;
}

/**
 * @brief Handle error if raised, otherwise, do nothing
 * @details 
 * @param u16_Event event to process
**/
static void cyclicTask_HandleError( uint16_t u16_Event )
{
  if( (u16_Event & MASK_TASK_ERROR) != 0 )
  {
    
    TASK_ClearEvent( MASK_TASK_ERROR );
    
    if( Var_GetVar_u32( VAR_ID_ERROR,last_value) != 0 )
    {
      // Error Detected
      Var_SetVar_u8( VAR_ID_LED_OK, 0u );
      Var_SetVar_u8( VAR_ID_LED_ERROR, 1u );
      HAL_Relay_Setoff();  
    }
    else
    {
       // Error Detected
      Var_SetVar_u8( VAR_ID_LED_OK, 1u );
      Var_SetVar_u8( VAR_ID_LED_ERROR, 0u );
    }
  }
}

/**
 * @brief Update LED according to variables
 * @details
 * @param u16_Event event to process
**/
static void cyclicTask_UpdateHmi( uint16_t u16_Event )
{  
  if( (u16_Event & MASK_TASK_UPDATE_HMI)!= 0 )
  {
    TASK_ClearEvent( MASK_TASK_UPDATE_HMI); 
    // Update OK Led
    if( Var_GetVar_u8 (VAR_ID_LED_OK,last_value) != 0u )
    {
      HAL_LedOK_TurnOn();       
    }
    else
    { HAL_LedOK_TurnOff();     
    }

    // Update Warning LED
    if( Var_GetVar_u8 (VAR_ID_LED_WARNING,last_value ) != 0u )
    { 
      HAL_LedWarning_TurnOn();  
    }
    else
    { 
      HAL_LedWarning_TurnOff(); 
    }

    // Update Error LED
    if( Var_GetVar_u8 (VAR_ID_LED_ERROR,last_value) != 0u )
    { 
      HAL_LedError_TurnOn();    
    }
    else
    { 
      HAL_LedError_TurnOff();   
    }

    // Update 7 bar
    if(Var_GetVar_u8 (VAR_ID_POT_LEVEL,last_value) != Var_GetVar_u8 (VAR_ID_POT_LEVEL,previous_last) )
    {
      HAL_UpdateDisplay(Var_GetVar_u8(VAR_ID_POT_LEVEL,last_value));
    }
  }
}

/**
 * \fn void cyclicTask_Motor( uint32_t u16_Event )
 * \brief Update motor variable (Voltage and current)
 * \details
**/
static void cyclicTask_GetMonitorMotor( uint16_t u16_Event )
{
  if( (u16_Event & MASK_TASK_MONITOR_MOTOR) != 0 )
  {
    TASK_ClearEvent( MASK_TASK_MONITOR_MOTOR );

    Var_SetVar_u32( VAR_ID_VOLT_SUPPLY, HAL_Mesure_Tension_alim() );
    Var_SetVar_u32( VAR_ID_CUR_MOTOR, HAL_Mesure_Courant_motor() );   
    Var_SetVar_u8(VAR_ID_CAP_MVM_1,HAL_CaptMov1_Get());
    Var_SetVar_u8(VAR_ID_CAP_MVM_2,HAL_CaptMov2_Get());
  }
}

/**
 * \fn void cyclicTask_GetMicroSwitch( uint32_t u16_Event )
 * \brief Update microswitch status
 * \details
**/
static void cyclicTask_GetMicroSwitch( uint16_t u16_Event )
{
  uint8_t u8_switchValue;
  if( (u16_Event & MASK_TASK_MICRO_SW) != 0 )
  {
    TASK_ClearEvent( MASK_TASK_MICRO_SW );
      
    Var_SetVar_u8(VAR_ID_SW_SYRING1,HAL_Switch_PS1_Get());
    // Var_SetVar_u8(VAR_ID_SW_SYRING2,HAL_Switch_PS2_Get());
    Var_SetVar_u8(VAR_ID_SW_REV,HAL_Switch_AR_Get());
    Var_SetVar_u8(VAR_ID_SW_ENDPOS1,HAL_Switch_AV1_Get());
    Var_SetVar_u8(VAR_ID_SW_ENDPOS2,HAL_Switch_AV2_Get());
  }
}

/**
 * @brief Update desk button BP status and potentiometer
 * @details
**/
static void cyclicTask_GetHmiEvent( uint16_t u16_Event )
{
  if( (u16_Event & MASK_TASK_GET_HMI) != 0 )
  {
    TASK_ClearEvent( MASK_TASK_GET_HMI );

    Var_SetVar_u8( VAR_ID_BB_START, HAL_ButtonStart_Get());
    Var_SetVar_u8( VAR_ID_BB_REV, HAL_ButtonReverse_Get());
    Var_SetVar_u8( VAR_ID_POT_LEVEL, HAL_Potentiometre());
  }
}

/*--------------------------------------------------------------------------*/
/* ... EXT FUNCTIONS ...                                                    */
/*--------------------------------------------------------------------------*/

/**
 * \fn void TASK_Init( void )
 * \brief 
 * \details
**/
void TASK_Init( void )
{
  Var_Init();
}

/**
 * @brief Set task event mask
 * @details
 * @param u32_tick tick number
**/
void TASK_ToggleEvent( uint32_t u32_tick )
{
  uint32_t u32_localTick;
  
  u32_localTick = ( u32_tick % 20 );
  
  switch( u32_localTick )
  {
    /* 50%  */
    case 0:
    case 2:
    case 4:
    case 6:
    case 8:
    case 10:
    case 12:
    case 14:
    case 16:
    case 18:
      gu16_EventTask |= MASK_TASK_ERROR;       
      gu16_EventTask |= MASK_TASK_MONITOR_MOTOR; 
      break; 
    /* 35% */  
    case 1:
    case 5:
    case 7:
    case 11:
    case 15:
    case 17:
    case 19: 
      gu16_EventTask |= MASK_TASK_GET_HMI;
      gu16_EventTask |= MASK_TASK_MICRO_SW;
      gu16_EventTask |= MASK_TASK_UPDATE_HMI;
      break; 
    /* 15% */  
    case 3:
    case 9:
    case 13:
      break;
  }
}

/**
 * @brief call all cyclic task
 * @details
 *  call cyclicTask_GetMonitorMotor() function with u32_EventTask parameter
 *  call cyclicTask_HandleError() function with u32_EventTask parameter
 *  call cyclicTask_UpdateHmi() function with u32_EventTask parameter
 *  call cyclicTask_Display() function with u32_EventTask parameter
 *  call cyclicTask_Motor() function with u32_EventTask parameter
 *  call cyclicTask_GetMicroSwitch() function with u32_EventTask parameter
 *  call cyclicTask_GetHmiEvent() function with u32_EventTask parameter
 * @param u32_EventTask flag for the func to be executed (conf in TASK_ToggleEvent)
**/
void TASK_ManageEvent()
{
  cyclicTask_GetMonitorMotor( gu16_EventTask );
  cyclicTask_HandleError( gu16_EventTask );
  cyclicTask_UpdateHmi( gu16_EventTask );
  cyclicTask_GetMicroSwitch( gu16_EventTask );
  cyclicTask_GetHmiEvent( gu16_EventTask );
}





