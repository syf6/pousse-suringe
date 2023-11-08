/**
 * @file Var.h
 * @author pierre.denis
 * @brief Memory table
**/
#ifndef _VAR_H_
#define _VAR_H_


/*--------------------------------------------------------------------------*/
/* ... INCLUDES ...                                                         */
/*--------------------------------------------------------------------------*/

#include <Arduino.h>

/*--------------------------------------------------------------------------*/
/* ... DATAS TYPES ...                                                      */
/*--------------------------------------------------------------------------*/

#define ERROR_NO_ERROR          0x00000000u
#define ERROR_SYRINGE           0x00000001u
#define ERROR_STOPBUTTON        0x00000002u
#define ERROR_OVERCURRENT       0x00000004u


typedef enum{
  last_value,
  previous_last,
} DataAge;

typedef enum
{
  VAR_ID_ERROR = 0,
  VAR_ID_VOLT_SUPPLY,
  VAR_ID_CUR_MOTOR,
  VAR_ID_POT_RAW,
  VAR_ID_32_MAX,
}E_IDX_VAR_32;

/*! Index List uint8_t variable*/
typedef enum
{
   VAR_ID_LED_OK = 0,   /*!< led OK setting variable                 */
   VAR_ID_LED_ERROR,    /*!< led error setting variable              */
   VAR_ID_LED_WARNING,  /*!< led warning setting variable            */
   VAR_ID_SW_SYRING1,   /*!< status of first switch seringe presence */
   VAR_ID_SW_SYRING2,   /*!< status of second switch seringe presenc  */
   VAR_ID_SW_REV,       /*!< status reverse switch seringe presence  */
   VAR_ID_SW_ENDPOS1,   /*!< status first end point switch           */
   VAR_ID_SW_ENDPOS2,   /*!< status second end point switch          */
   VAR_ID_BB_START,     /*!< Start button status variable             */
   VAR_ID_BB_REV,       /*!< Reverse button status variable           */
   VAR_ID_POT_LEVEL,    /*!< Potentiometer value           */
   VAR_ID_PSH_SRG,      /*!< Flag which indicate if the syringue is handled */
   VAR_ID_CAP_MVM_1,    /*!< Hall Sensor B */
   VAR_ID_CAP_MVM_2,    /*!< Hall Sensor A */
   VAR_ID_08_MAX
}E_IDX_VAR_08;


/*--------------------------------------------------------------------------*/
/* ... DATAS ...                                                            */
/*--------------------------------------------------------------------------*/

extern uint32_t ga_u32CurVar[VAR_ID_32_MAX];
extern uint32_t ga_u32OldVar[VAR_ID_32_MAX];

extern uint8_t ga_u8CurVar[VAR_ID_08_MAX];
extern uint8_t ga_u8OldVar[VAR_ID_08_MAX];

/*--------------------------------------------------------------------------*/
/* ... FUNCTIONS ...                                                        */
/*--------------------------------------------------------------------------*/

extern void       Var_Init( void );
/* Setter */
extern void       Var_SetVar_u8  ( E_IDX_VAR_08 e_IdxVar, uint8_t u8Value );
extern void       Var_SetVar_u32 ( E_IDX_VAR_32 e_IdxVar, uint32_t u32Value );
/* Getter */
extern uint8_t    Var_GetVar_u8  ( E_IDX_VAR_08 e_IdxVar, DataAge age);
extern uint32_t   Var_GetVar_u32 ( E_IDX_VAR_32 e_IdxVar, DataAge age );
/* Error Handler */
extern void Var_SetError( uint32_t u32_MaskError );
extern void Var_ClearError();

#endif
