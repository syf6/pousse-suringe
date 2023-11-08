/**
 * \file FSM.c
 * \author pierre.denis
 * \brief Regulation
**/

#ifndef _REG_H_
#define _REG_H_

/*--------------------------------------------------------------------------*/
/* ... INCLUDES ...                                                         */
/*--------------------------------------------------------------------------*/

#include <Arduino.h>

/*--------------------------------------------------------------------------*/
/* ... FUNCTIONS ...                                                        */
/*--------------------------------------------------------------------------*/
extern void REG_Init();
extern int  REG_pid_vitesse( void );
extern int  REG_pid_debit( void ); 
extern bool REG_analyzeCurrent(uint32_t incomingCurrent);
#endif
