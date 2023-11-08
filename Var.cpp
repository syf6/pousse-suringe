/**
 * @file Var.c
 * @author pierre.denis
 * @brief Memory table
**/

/*--------------------------------------------------------------------------*/
/* ... INCLUDES ...                                                         */
/*--------------------------------------------------------------------------*/
#include "Var.hpp"

/*--------------------------------------------------------------------------*/
/* ... DATAS ...                                                            */
/*--------------------------------------------------------------------------*/

uint32_t  ga_u32CurVar[VAR_ID_32_MAX];
uint32_t  ga_u32OldVar[VAR_ID_32_MAX];
uint8_t   ga_u8CurVar[VAR_ID_08_MAX];
uint8_t   ga_u8OldVar[VAR_ID_08_MAX];
/*--------------------------------------------------------------------------*/
/* ... FUNCTIONS ...                                                        */
/*--------------------------------------------------------------------------*/

/**
 * \fn void Var_Init( void )
 * \brief Init all variables array value with default value
 * \details Set to 0 value ga_u32CurVar array
 *          Set to 0 value ga_u32OldVar array
 *          Set to 0 value ga_u16CurVar array
 *          Set to 0 value ga_u16OldVar array
 *          Set to 0 value ga_u8CurVar array
 *          Set to 0 value ga_u8OldVar array  
**/
void Var_Init( void )
{
  uint8_t u8_idx;

  for( u8_idx=0; u8_idx < VAR_ID_32_MAX; u8_idx++ )
  {
    ga_u32CurVar[u8_idx] = 0;
    ga_u32OldVar[u8_idx] = 0;
  }

  for( u8_idx=0; u8_idx < VAR_ID_08_MAX; u8_idx++ )
  {
    ga_u8CurVar[u8_idx] = 0;
    ga_u8OldVar[u8_idx] = 0;
  }
}

/**
 * \fn void Var_SetVar_u8( E_IDX_VAR_08 e_IdxVar, uint8_t u8Value )
 * \brief Set one variable selected by it index
 * \details Set ga_u8OldVar variable
 *          Set ga_u8CurVar variable
**/
void Var_SetVar_u8( E_IDX_VAR_08 e_IdxVar, uint8_t u8Value )
{
  if( e_IdxVar < VAR_ID_08_MAX )
  {
    ga_u8OldVar[e_IdxVar] = ga_u8CurVar[e_IdxVar];
    ga_u8CurVar[e_IdxVar] = u8Value;
  }
}

/**
 * \fn void void Var_SetVar_u32( E_IDX_VAR_32 e_IdxVar, uint32_t u32Value )
 * \brief 
 * \details
**/
void Var_SetVar_u32( E_IDX_VAR_32 e_IdxVar, uint32_t u32Value )
{
  if( e_IdxVar < VAR_ID_32_MAX )
  {
    ga_u32OldVar[e_IdxVar] = ga_u32CurVar[e_IdxVar];
    ga_u32CurVar[e_IdxVar] = u32Value;
  }
}

/**
 * \fn uint8_t Var_GetVaru8( E_IDX_VAR_08 e_IdxVar )
 * \brief 
 * \details
**/
uint8_t Var_GetVar_u8( E_IDX_VAR_08 e_IdxVar, DataAge age)
{
  uint8_t u8_ReturnValue = 0xFF;

  if( e_IdxVar < VAR_ID_08_MAX )
  {
    if (age == last_value)
      u8_ReturnValue = ga_u8CurVar[e_IdxVar];
    else
      u8_ReturnValue = ga_u8OldVar[e_IdxVar];
  }
  return( u8_ReturnValue );
}

/**
 * \fn uint32_t Var_GetVar_u32( E_IDX_VAR_32 e_IdxVar )
 * \brief 
 * \details
**/
uint32_t Var_GetVar_u32( E_IDX_VAR_32 e_IdxVar , DataAge age)
{
  uint32_t u32_ReturnValue;
  u32_ReturnValue = 0xFF;
  if( e_IdxVar < VAR_ID_32_MAX )
  {
    if (age == last_value)
      u32_ReturnValue = ga_u32CurVar[e_IdxVar];
    else
      u32_ReturnValue = ga_u32OldVar[e_IdxVar];
  }
  return( u32_ReturnValue );
}

/**
 * @brief Add error without erasing active one
 * @details
**/
void Var_SetError( uint32_t u32_MaskError )
{
  uint32_t u32_Error;
  
  u32_Error = Var_GetVar_u32(VAR_ID_ERROR,last_value);
  u32_Error |= u32_MaskError;
  Var_SetVar_u32(VAR_ID_ERROR, u32_Error);
}

/**
 * @brief Clear Error
 * @details
**/
void Var_ClearError()
{  
  Var_SetVar_u32(VAR_ID_ERROR, ERROR_NO_ERROR);
}