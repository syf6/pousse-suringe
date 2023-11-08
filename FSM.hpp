/**
 * \file FSM.h
 * \author pierre.denis
 * \brief Finite state machine
**/

#ifndef _FSM_H_
#define _FSM_H_

/*--------------------------------------------------------------------------*/
/* ... INCLUDES ...                                                         */
/*--------------------------------------------------------------------------*/
#include <Arduino.h>

/*--------------------------------------------------------------------------*/
/* ... DATAS TYPES ...                                                      */
/*--------------------------------------------------------------------------*/

typedef enum
{
  FSM_BOOT = 0,
  FSM_IDLE,
  FSM_INSTALLATION,
  FSM_WAIT,
  FSM_RUN,
  FSM_REMOVE,
  FSM_ERROR,
  FSM_STATE_MAX,
}E_FSM;

extern E_FSM g_eFSM_State;

/*--------------------------------------------------------------------------*/
/* ... PROTO ...                                                            */
/*--------------------------------------------------------------------------*/

extern E_FSM FSM_Error( void );
extern E_FSM FSM_RemoveDevice( void );
extern E_FSM FSM_Run( void );
extern E_FSM FSM_Wait( void );
extern E_FSM FSM_Idle( void );
extern E_FSM FSM_Installation( void );
extern E_FSM FSM_Boot( void );
extern void FSM_DetectError( void );

extern void FSM_Init( void );
extern void FSM_Handler( void );
extern E_FSM FSM_GetCurrentState(void);
extern E_FSM FSM_GetPreviousState(void);

#endif
