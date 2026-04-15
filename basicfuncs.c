#include "basicfuncs.h"
#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_systick.h>
#include <nrf_gpio.h>
#include <stdlib.h>
#include <nrfx_rtc.h>
#include <string.h>
#include <nrfx_uarte.h>

//Definierar vilka pinnar som ska användas för att skicka och ta emot data:
#define PIN_TXD 20
#define PIN_RXD 22

//Används endast internt, så ingen extern
nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);
nrfx_rtc_t rtc_instance = NRFX_RTC_INSTANCE(0);

//en funktion för att skicka strängen data av storlek length, över UARTE
void uarte_write(char* data, int length)
{
	nrfx_uarte_tx(&instance, data, length, 0);  //Skriver ut till uarte
}

//Läser en input string; bokstav för bokstav
void read_string(char *inArray, int maxLen)
{
	int i = 0;  //Skapar int
	char currentInput;  //Skapar char

	do
	{
		//Vi väntar på att läsa ett tecken över UARTE
		nrfx_uarte_rx (&instance, &currentInput, 1); //Läser in tecken med storlek en byte
		if(currentInput!='\r')
		{
			inArray[i]=currentInput;    //Lägger currentInputen i in Array
			i++;    //Indexerar
		}
	} while((i<maxLen) && (currentInput != '\r'));
	inArray[i]='\0';    //Lägger till \0 som sista tecken
}

//Läs en input strin och konverterar till int; av storlek int-parametern
int read_int(int maxLen)
{
	char teckenArray[maxLen+1];    //Skapar char sträng
	read_string(teckenArray, maxLen);   //Tar teckenArray som input och fyller den från från funktionen
	return atoi(teckenArray);   //lämnar tillbaka
}

//Skriver ett heltal till terminalen
void send_int(int inputNumber)
{
	char outputBuffer[20];   //Skapar char sträng
	sprintf(outputBuffer, "%d \r\n", inputNumber);   //Konverterar inputNumber till string och lägger den i outputBuffer
	uarte_write(outputBuffer, strlen(outputBuffer));  //Skriver ut buffern med storlek av längden gånger 1 byte
}

//Initierar kortet med RTC, systick, knappar och lampor
void init_stuff(void)
{
    //Skapar en driver instance för UARTE:
    
    //Initierar systick:
    nrfx_systick_init();
	//Initierar output:
	nrf_gpio_cfg_output(LED1);
	nrf_gpio_cfg_output(LED2);
	nrf_gpio_cfg_output(LED3);
	nrf_gpio_cfg_output(LED4);
	//Släck alla lampor:
	nrf_gpio_pin_write(LED1, LED_OFF);
	nrf_gpio_pin_write(LED2, LED_OFF);
	nrf_gpio_pin_write(LED3, LED_OFF);
	nrf_gpio_pin_write(LED4, LED_OFF);
	//Initierar input:
	nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(BUTTON2, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(BUTTON3, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(BUTTON4, NRF_GPIO_PIN_PULLUP);
    //Initierar UARTE:
    nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);
	nrfx_err_t errr = nrfx_uarte_init(&instance, &config, NULL);
    //Initierar rtc:
    nrfx_rtc_config_t rtc_config = NRFX_RTC_DEFAULT_CONFIG;
    nrfx_err_t errr_rtc = nrfx_rtc_init(&rtc_instance, &rtc_config, NULL);
    nrfx_rtc_enable(&rtc_instance);
}

//Tänder Lampor
void led_write(int LED_ID, int STATE)
{
	nrf_gpio_pin_write(LED_ID, STATE);
}

//Läser knappar och returnerar deras värde
int button_read(int buttonNumber)
{
	return nrf_gpio_pin_read(buttonNumber);
}

//Delay i sekunder
void delay_s(int time)	
{
	nrfx_systick_delay_ms(time*1000);
}

//Delay i millisekunder
void delay_ms(int time)	
{
	nrfx_systick_delay_ms(time);
}