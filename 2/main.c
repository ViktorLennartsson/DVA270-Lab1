#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_systick.h>
#include <nrf_gpio.h>

#define LED1 28
#define LED2 29
#define LED3 30
#define LED4 31
#define BUTTON1 23
#define BUTTON2 24
#define BUTTON3 8
#define BUTTON4 9
#define LED_OFF 1
#define LED_ON 0
#define PIN_HIGH 1
#define PIN_LOW 0


int button_read(int buttonNumber)	//Läser knappar och returnerar deras värde
{
	return nrf_gpio_pin_read(buttonNumber);
}


void led_write(int LED, int STATUS)	//Tänder Lampor
{
	nrf_gpio_pin_write(LED, STATUS);
}


void changeLight(int LED, int *litStatus)	//Ändrar lampor
{
	int LED_ID;
	//Få rätt ID på LEDen baserat på indexering
	switch(LED)
	{
	case 0: LED_ID = LED1; break;
	case 1: LED_ID = LED2; break;
	case 2: LED_ID = LED3; break;
	case 3: LED_ID = LED4; break;
	default: return;
	}

	//Om LED på -> LED av, LED av -> LED på
	if(litStatus[LED]==LED_OFF)
	{
		led_write(LED_ID, LED_ON);
		litStatus[LED] = LED_ON;
	}
	else
	{
		led_write(LED_ID, LED_OFF);
		litStatus[LED] = LED_OFF;
	}
	
}

void init_gpio(void)	//INIT
{
	nrfx_systick_init();
	//Init output
	nrf_gpio_cfg_output(LED1);
	nrf_gpio_cfg_output(LED2);
	nrf_gpio_cfg_output(LED3);
	nrf_gpio_cfg_output(LED4);
	//Släck alla lampor
	nrf_gpio_pin_write(LED1, LED_OFF);
	nrf_gpio_pin_write(LED2, LED_OFF);
	nrf_gpio_pin_write(LED3, LED_OFF);
	nrf_gpio_pin_write(LED4, LED_OFF);
	//Init input
	nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(BUTTON2, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(BUTTON3, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(BUTTON4, NRF_GPIO_PIN_PULLUP);
}

void button_status(int *status)	//Kollar status för alla knappar
{
	//Indexerar nuvarande status i en lista
	status[0] = button_read(BUTTON1);
	status[1] = button_read(BUTTON2);
	status[2] = button_read(BUTTON3);
	status[3] = button_read(BUTTON4);
}

void delay(void)	//Delay
{
	nrfx_systick_delay_ms(50);
}

void changeAndLight(int *oldStatus, int *newStatus, int *lights)	//Kollar om statusen för någon knapp har ändrats
{
	//Genomför check för varje index i status
	for(int i = 0; i < 4; i++)
    {
		//Kollar specifikt om man trycker NER en knapp
        if(oldStatus[i] == PIN_HIGH && newStatus[i] == PIN_LOW)
        {
			//Om knapp 'i' har förändrats, ändra lampa 'i' baserat på statusen i 'lights'
            changeLight(i, lights);
        }
		//Lampan är up-to-speed inga förändringar behöver förvaras
		oldStatus[i] = newStatus[i];
	}
}

int main(void)	//Kör programet
{
	//Init
	init_gpio();
	//Skapa variabler
	int buttonStatus0[]={PIN_HIGH, PIN_HIGH, PIN_HIGH, PIN_HIGH};
	int buttonStatus1[]={PIN_HIGH, PIN_HIGH, PIN_HIGH, PIN_HIGH};
	int lightStatus[]={LED_OFF, LED_OFF, LED_OFF, LED_OFF};
	//Main loop med funktioner
	while(1)
	{
		//Sakta ner programmet så det inte kör för snabbt
		delay();
		//Kolla status på knappar
		button_status(buttonStatus1);
		//Kolla om någon knapp har ändrats och tänd respektive lampor
		changeAndLight(buttonStatus0, buttonStatus1, lightStatus);
	}
}
