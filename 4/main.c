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

#define BUTTON1 23
#define PIN_HIGH 1
#define PIN_LOW 0

//definierar vilka pinnar som ska användas för att skicka och ta emot data:
#define PIN_TXD 20
#define PIN_RXD 22

const nrfx_rtc_t rtc_instance = NRFX_RTC_INSTANCE(0);

//Skapar en driver instance för UARTE:
nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);

//en funktion för att skicka strängen data av storlek length, över UARTE
void uarte_write(char* data, int length)
{
	nrfx_uarte_tx(&instance, data, length, 0);  //Skriver ut till uarte
}

void init_gpio(void)	//INIT
{
	nrfx_systick_init();
	nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
}

int button_read(int buttonNumber)	//Läser knappar och returnerar deras värde
{
	return nrf_gpio_pin_read(buttonNumber);
}

void delay(void)	//Delay
{
	nrfx_systick_delay_ms(50);
}

int main(void)
{
    init_gpio();
    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);
	//Initierar UARTE:
	nrfx_err_t errr = nrfx_uarte_init(&instance, &config, NULL);
	if (errr != 0){
	//Här kan vi implementera felhantering, men för stunden håller vi tummarna att allt funkar
	}
    //Initierar systick:
    nrfx_systick_init();

    nrfx_rtc_config_t rtc_config = NRFX_RTC_DEFAULT_CONFIG;
    nrfx_err_t errr_rtc = nrfx_rtc_init(&rtc_instance, &rtc_config, NULL);
    nrfx_rtc_enable(&rtc_instance);
    
    char str[20];
    int status0 = PIN_HIGH;
    int status1 = PIN_HIGH;
    char msg[] = "\n\r Tryck på för att skriva ut slumptal. \n\r";
    uarte_write(msg, strlen(msg));
    while(1)
    {
        delay();
        status1 = button_read(BUTTON1);
        if(status0 == PIN_HIGH && status1 == PIN_LOW)
        {
            srand(nrfx_rtc_counter_get(&rtc_instance));
            sprintf(str, "%d\n\r", (rand()%10000)+1);
            uarte_write(str, strlen(str));
        }
        status0 = status1;
    }
}