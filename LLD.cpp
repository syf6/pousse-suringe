/**
 * \file FSM.c
 * \author pierre.denis
 * \brief Low Layer
**/

/**\page LLD_Page Low level Device  Class
 * \section sectionDescription_LLD Description
 * LLD class allow task event management and launch appropriate phase with global variable ge_FSM_Phase
 * This class consists of the following files :
 * - LLD.c
*/

/*--------------------------------------------------------------------------*/
/* ... INCLUDES ...                                                         */
/*--------------------------------------------------------------------------*/
#include <arduino.h>
#include "LLD.hpp" 

/*--------------------------------------------------------------------------*/
/* ... DATAS ...                                                        */
/*--------------------------------------------------------------------------*/
uint32_t gu32_Tick;

/*--------------------------------------------------------------------------*/
/* ... FUNCTIONS ...                                                        */
/*--------------------------------------------------------------------------*/

/**
 * \fn void LLD_PinInit( void )
 * \brief Initialize all pins
 * \details
**/
void LLD_PinInit( void )
{

  gu32_Tick = 0;
    
  pinMode(0, INPUT_PULLUP);    // D0 => FDC AV2
  pinMode(1, INPUT_PULLUP);    // D1 => PS2
  pinMode(2, INPUT_PULLUP);    // D2 => SENS_MOTEUR 1
  pinMode(3, INPUT_PULLUP);    // D3 => SENS_MOTEUR 2
  pinMode(4, OUTPUT);          // D4 => OK LED
  pinMode(6, INPUT_PULLUP);    // D5 => BP_Start
  pinMode(7, OUTPUT);          // D6 => Relay Static: bobine relais  
  pinMode(8, OUTPUT);          // D8 => Frein moteur B 
  pinMode(9, OUTPUT);          // D9 => Led_error 
  pinMode(10, OUTPUT);         // D10 => Led_warning
  pinMode(11, OUTPUT);         // D11 => DIR Motor B 
  pinMode(12, INPUT_PULLUP);   // D12 => BP Retour seringue en pullup attention BP normalement fermé
  pinMode(13, OUTPUT);         // D13 => PWM  moteur B  
  pinMode(A0, INPUT_PULLUP);   // A0 => PT_DIV_MOT Mesure de la tension moteur
  pinMode(A1, INPUT_PULLUP);   // A1 => FDC AV1 (FDC Avant 1)
  pinMode(A2, INPUT);          // A2 => Potentiomètre (Mesure de la tension potentiometre)
  pinMode(A3, INPUT);          // A3 => SNS 1 (Mesure de la tension d'alimentation)
  pinMode(A4, INPUT_PULLUP);   // A4 => FDC_AR (Butée Arrière) 
  pinMode(A5, INPUT_PULLUP);   // A5 => PS1 (Presence seringue 1) 
  
  // Activation tension moteur
  digitalWrite(7, HIGH);       
}

/**
 * \fn void LLD_TimerInit( void )
 * \brief initialize Timer1
 * \details
**/
void LLD_TimerInit( void )
{ 
  // disable all interrupts
  cli();  

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  
  //Value for ORC1A for 1ms 
  OCR1A=0xF9;               
  //Set the prescale 1/64 clock
  TCCR1B |= (1<<WGM12);     
  TCCR1B |= (1<<CS12) |(1<<CS10);
  //Set the interrupt request
  TIMSK1 |= (1<<OCIE1A);   
  //Enable interrupt
  sei(); 
  // enable all interrupts
  interrupts(); 
}