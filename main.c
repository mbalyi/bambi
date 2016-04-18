

#include "mcu_avr_atmega128_api.h" 	// MCU API   
#include "dpy_trm_s01.h"			// DPY API

/********  main program  ***************************/
int main (void)
{
   dpy_trm_s01__Init();		// Initialize the DPY dysplay card
   Timer0_Init();			// Initialize timer0
  
							
   SYS_LED_DIR_OUTPUT();	// Set the pin driving the system led to output
   SYS_LED_ON();			// Switch on system led
   //sei();					// enable interrupts
   

   
   while(DPY_TRM_S01__BUTTON_2_GET_STATE()){
   randNum=generateNum();
   }

   while(1)
   {
	start();
   }
}


/********  Timer0 initialisation  ***************************/
void Timer0_Init(void)
{
   TCCR0=0x07;			// Set TIMER0 prescaler to CLK/1024
   TCNT0=0;				// Set the counter initial value                    
  // TIMSK=_BV(TOIE0);	// Enable TIMER0 overflow interrupt                 
}

