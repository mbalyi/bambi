#include "mcu_avr_atmega128_api.h" 	// MCU API   
#include "dpy_trm_s01.h"			// DPY API
#include "game.h"

// uint16_t (pergesmentesites ciklus hossza)
#ifndef Bounce
#define Bounce 0x007F	//számláló elszámolási ciklusa
#endif

// Interrupt 200MS-re valtasa
#ifndef INT_200MS_COUNT	//idő osztva interruptidő
#define INT_200MS_COUNT 4	//50ms-enként váltás és jelfigyelés
#endif

// Interrupt 2 sec-re valtasa (10*INT_200MS_COUNT)
#ifndef INT_2S_COUNT
#define INT_2S_COUNT 40
#endif

// Szamlalo pergesmentesiteshez
uint16_t debounce[3] = {Bounce, Bounce, Bounce};

int tip=0;			//a kezdeti tippünk definiálása 
int cnt1=0;
int cnt3=0;
float randNum;
unsigned char button1, button2, button3;

void generateNum(){
srand(TCNT0);
randNum=rand() % 1000;
}

void buttons(){
	button1=DPY_TRM_S01__BUTTON_1_GET_STATE();
	button2=DPY_TRM_S01__BUTTON_2_GET_STATE();
	button3=DPY_TRM_S01__BUTTON_3_GET_STATE();
}

void leds_on(){
	DPY_TRM_S01__LED_1_ON();
	DPY_TRM_S01__LED_2_ON();
	DPY_TRM_S01__LED_3_ON();
	DPY_TRM_S01__LED_4_ON();
}

void leds_off(){
	DPY_TRM_S01__LED_1_OFF();
	DPY_TRM_S01__LED_2_OFF();
	DPY_TRM_S01__LED_3_OFF();
	DPY_TRM_S01__LED_4_OFF();
}

/* Összehasonlító függvény */
void compare(int calc, int random){
	button1=button2=button3=1;
	if(calc < random){	//ha a tippelt szám kisebb a random számnál
		while(button1 && button2 && button3) {		//végtelenített ciklusban menjen a villogás egy interruptig.
			DPY_TRM_S01__LED_4_ON();	//ha kisebb akkor 4-es LED-et bekapcsoljuk
			_delay_ms(200);				//200ms-ig hagyjuk égni
			DPY_TRM_S01__LED_4_OFF();	//kikapcsoljuk a 4-es LED-et
			_delay_ms(200);			//200ms-ig nem világít. Így végtelenített ciklusban villog.
			buttons();
		}
	}
	/* ugyanazon gondolatmenettel mint az elobb */
	if(calc > random){
		while(button1 && button2 && button3) {
			DPY_TRM_S01__LED_1_ON();
			_delay_ms(200);
			DPY_TRM_S01__LED_1_OFF();
			_delay_ms(200);
			buttons();
		}
	}
	/* ha eltaláltuk a számot akkor minden LED villogjon */
	/* ugyan úgy mint eddig, csak az összes LED-et állítjuk */
	if(calc == random){
		while(button1 && button2 && button3) {
			leds_on();
			_delay_ms(200);
			leds_off();
			_delay_ms(200);
			buttons();
		}
	}
}

void start(){
	buttons();
	
	unsigned char previous_state;
	
	/* ha a jobb oldali gombot nyomjuk akkor növeljük az értéket */
	if((!button1){
		
		//previous_state=debounce[0];
		
		if(cnt1 > 20)
			tip+=15;
		else if(cnt1 > 10)
			tip+=5;
		else if(cnt1 > 6)
			tip+=2;
		else tip++;
		cnt1++;
	}
	else cnt1=0;
		
	/* középso gomb megnyomásával összehasonlítjuk a tippelt számot a random számmal */
	if(!button2){
		_delay_ms(100);
		compare(tip,(int)randNum);
		}

	/* ha a bal oldali gombot nyomjuk akkor csökkentjük az értéket */
	if((!button3){
		
		//previous_state=debounce[2];
		
		if(cnt3 > 20)
			tip-=15;
		else if(cnt3 > 10)
			tip-=5;
		else if(cnt3 > 6)
			tip-=2;
		else tip--;
		cnt3++;
	}
	else cnt3=0;
		
	dpy_trm_s01__7seq_write_number(tip, 0);	//minden ciklusban kiiratjuk a hétszegmens kijelzore a tippet.
	_delay_ms(100);
		
}


