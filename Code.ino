/**
 * @file PousseSeringue_ISYMED_DBG.ino
 * @author pierre.denis
 * @brief Main file
**/


/** @mainpage PushSeringe : Push Seringe
 *  @section introduction Introduction
 *  @image html synoptic.png
 *  @image latex synoptic.png "Synoptique" width=5cm
 *   This code is used to manage push seringe \n
 *   
 *   This document describes the following elements:
 *  - @ref FSM_Page                   "Finite State Class"              \n
 *  - @ref TASK_Page                  "Cyclic Task Class"               \n
 *  - @ref REG_Page                   "Regulation Class"                \n
 *  - @ref HAL_Page                   "Hard Abstractive Layer Class "   \n
 *  - @ref LLD_Page                   "Low Level Layer Class"           \n
 *  - @ref VAR_Page                   "List of variable Class"          \n
**/

/*--------------------------------------------------------------------------*/
/* ... INCLUDES ...                                                         */
/*--------------------------------------------------------------------------*/
#include <arduino.h>
#include <stdint.h>

#include "Debug.hpp"
#include "Var.hpp"
#include "LLD.hpp"
#include "HAL.hpp"
#include "TASK.hpp"
#include "FSM.hpp"
#include "Var.hpp"
#include "REG.hpp"

/**
 * \fn ISR(TIMER1_COMPA_vect)
 * \brief Timer 1 IRQ call back
 * \details increment gu32_Tick Var
**/
ISR(TIMER1_COMPA_vect)
{    //This is the interrupt request
  gu32_Tick++;
}

/**
 * @brief Init: SM, pins, variables values & timer 
 * @details
**/
void setup( void ) 
{
  Serial.println( "BOOTING APPLICATION");

  LLD_TimerInit();
  HAL_Init();
  REG_Init();
  TASK_Init();
  FSM_Init();

  Serial.begin(115200);
  Serial.flush();
}

/**
 * @brief Main loop processing
 * @details
**/
void loop( void )
{
  /* Handle Flag(s) for Cyclic tasks */
  TASK_ToggleEvent( gu32_Tick );
  /* Handle Cyclic tasks */
  TASK_ManageEvent();
  /* Handle FSM */
  FSM_Handler(); 

  /* Debug though Serial Monitor/Plotter*/
  // DebugMicroswitches();
  DebugButtons();
  // DebugState();
  // DebugMonitoring();
}
