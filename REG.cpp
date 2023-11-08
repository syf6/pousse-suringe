/**
 * \file FSM.c
 * \author pierre.denis
 * \brief Regulation
**/


/**\page REG_Page Regulation Class
 * \section sectionDescription_REG Description
 * REG class allow task event management and launch appropriate phase with global variable ge_FSM_Phase
 * This class consists of the following files :
 * - REG.c
*/

/*--------------------------------------------------------------------------*/
/* ... INCLUDES ...                                                         */
/*--------------------------------------------------------------------------*/
#include "REG.hpp"
#include "Var.hpp"
#include "HAL.hpp"

#define BUFFER_SIZE_CURRENT     150
#define OVERCURRENT_THRESHOLD   1500u

/*--------------------------------------------------------------------------*/
/* ... FUNCTIONS ...                                                        */
/*--------------------------------------------------------------------------*/
void REG_Init()
{
  
}

/**
 * \fn int REG_pid_vitesse( void )
 * \brief 
 * \details Qui sera maître dans une régulation de type cascade entre vitesse et débit?
**/
int REG_pid_vitesse( void )
{
  int sortie1;
  sortie1 = 0;
  // HAL_Mesure_Vitesse1();
  // mesure1=vit ;
  // //consigne1 = REG_REG_pid_debit(); // Si régulation de débit activée
  // ecart_n1 = consigne1 - mesure1 ;
  // somme_ecarts1 += ecart_n1 ;
  // var_ecarts1 = ecart_n1 - ecart_n_11 ;
  // commande1 = kp1*ecart_n1 + ki1*somme_ecarts1 + kd1*var_ecarts1 ;
  // if (commande1>255){
  //     commande1=255 ;}
  // else if (commande1<0){
  //     commande1=0 ;}
  // sortie1 = int(commande1);
  
  // ecart_n_11 = ecart_n1 ;
  return(sortie1);
}

/**
 * \fn int REG_pid_debit( void 
 * \brief 
 * \details Qui sera maître dans une régulation de type cascade entre vitesse et débit?
**/
int REG_pid_debit( void ) 
{
  int sortie2;
  // sortie2 = 0;
  //  mesure2=volume() ;
  //  ecart_n2 = consigne2 - mesure2 ;
  //  somme_ecarts2 += ecart_n2 ;
  //  var_ecarts2 = ecart_n2 - ecart_n_12 ;
  //  commande2 = kp2*ecart_n2 + ki2*somme_ecarts2 + kd2*var_ecarts2 ;
  //  if (commande2>1300){   // correspondance à une consigne de vitesse de 130 tr/min
  //      commande2=1300 ;}
  //  else if (commande2<0){
  //      commande2=0 ;}
  //  sortie2 = int(commande2);
   
  //  ecart_n_12 = ecart_n2 ;
   return(sortie2);
}

/**
 * @brief Post process current 
 * @details avoid false positive
 * @param incomingCurrent : new current
 * @param alarm: true if current problem, false otherwise
**/
bool REG_analyzeCurrent(uint32_t incomingCurrent)
{
  static uint8_t index = 0;
  static uint32_t current[BUFFER_SIZE_CURRENT] = {0};
  
  uint8_t counter = 0;
  bool alarm = false;
  
  // Store last value into the RB
  current[index] = incomingCurrent; 
  if (index < (BUFFER_SIZE_CURRENT-1))
    index++;
  else
    index = 0;

  // Analyze buffer
  for(uint8_t i = 0; i<BUFFER_SIZE_CURRENT; i++)
  {
    if (current[i]>OVERCURRENT_THRESHOLD)
      counter++;
    else
      break;
  }

  // Update alarm
  if (counter>=BUFFER_SIZE_CURRENT)
    alarm = true;

  return alarm;
}