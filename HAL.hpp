/**
 * \file FSM.c
 * \author pierre.denis
 * \brief Hardware Abstraction Layer
**/

#ifndef _HAL_H_
#define _HAL_H_ 

/*--------------------------------------------------------------------------*/
/* ... INCLUDES ...                                                         */
/*--------------------------------------------------------------------------*/
#include <arduino.h>
#include "Var.hpp"

/*--------------------------------------------------------------------------*/
/* ... FUNCTIONS ...                                                        */
/*--------------------------------------------------------------------------*/

extern void HAL_Init(void);

/*! @addtogroup HAL Led/IMH group
 * @{
 */
extern void   HAL_LedOK_TurnOn( void );
extern void   HAL_LedOK_TurnOff( void ); 
extern void   HAL_LedWarning_TurnOn( void );
extern void   HAL_LedWarning_TurnOff( void );
extern void   HAL_LedError_TurnOn( void ); 
extern void   HAL_LedError_TurnOff( void ); 
extern void   HAL_UpdateDisplay( uint8_t u8PosPotard );
extern void   HAL_UpdateDisplayError( uint8_t u8_error);

/*! @} */

/*! @addtogroup HAL Buttons/Potentiometer group
 * @{
 */
extern uint8_t  HAL_ButtonStart_Get( void ); 
extern uint8_t  HAL_ButtonReverse_Get( void );
extern uint8_t HAL_Potentiometre( void );
/*! @} */

/*! @addtogroup HAL Switch monitoring group
 * @{
 */
extern uint8_t  HAL_Switch_PS2_Get( void );
extern uint8_t  HAL_Switch_PS1_Get( void );
extern uint8_t  HAL_Switch_AV1_Get( void );
extern uint8_t  HAL_Switch_AV2_Get( void );
extern uint8_t  HAL_Switch_AR_Get( void );
extern uint8_t  HAL_CaptMov1_Get(void);
extern uint8_t  HAL_CaptMov2_Get(void);

/*! @} */

/*! @addtogroup HAL Motor control group
 * @{
 */
extern void   HAL_Relay_Seton( void );
extern void   HAL_Relay_Setoff( void );
extern uint32_t HAL_Mesure_Tension_alim( void );
extern uint32_t HAL_Mesure_Courant_motor( void );
extern void HAL_Forward_Motor( uint32_t u32Speed );
extern void HAL_Backward_Motor( void );
extern void HAL_Stop_Motor( void );
/*! @} */

#endif
