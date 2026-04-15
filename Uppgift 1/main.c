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
#define LED1 28
#define LED2 29
#define LED3 30
#define LED4 31
#define LED_OFF 1
#define LED_ON 0
#define MAXSIZE 10

//definierar vilka pinnar som ska användas för att skicka och ta emot data:
#define PIN_TXD 20
#define PIN_RXD 22

const nrfx_rtc_t rtc_instance = NRFX_RTC_INSTANCE(0);

//Skapar en driver instance för UARTE:
nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);

void led_write(int LED, int STATUS)	//Tänder Lampor
{
	nrf_gpio_pin_write(LED, STATUS);
}

void read_string(char *inArray)
{
    int i = 0;
    char c1;

    do
    {
        //Vi väntar på att läsa ett tecken över UARTE
        nrfx_uarte_rx (&instance, &c1, sizeof(c1));

        if(c1!='\r')
        {
            inArray[i]=c1;
            i++;
        }
        

        //Om vi lyckades läsa någonting så skickar vi tillbaka det som togs emot
        uarte_write(&c1, sizeof(c1));
    } while((i<MAXSIZE) && (c1 != '\r'));
    
    inArray[i]='\0';
    uarte_write(inArray, i);
}

//en funktion för att skicka strängen data av storlek length, över UARTE
void uarte_write(char* data, int length)
{
	nrfx_uarte_tx(&instance, data, length, 0);  //Skriver ut till uarte
}

int read_int(void)
{
	char teckenArray[MAXSIZE+1];    //Skapar char sträng
	read_string(teckenArray);   //Tar teckenArray som input och fyller den från från funktionen
	return atoi(teckenArray);   //lämnar tillbaka
}

void init_gpio(void)	//INIT
{
	nrfx_systick_init();
    //Init input
	nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
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

    //Initierar rtc:
    nrfx_rtc_config_t rtc_config = NRFX_RTC_DEFAULT_CONFIG;
    nrfx_err_t errr_rtc = nrfx_rtc_init(&rtc_instance, &rtc_config, NULL);
    nrfx_rtc_enable(&rtc_instance);
    
    //Skapar variabler
    char str[20];
    int status0 = PIN_HIGH;
    int status1 = PIN_HIGH;
    int delayTime;
    int loopAmount;
    int tempLight;
    //Meddelande
    char msg[] = "\n\r Tryck på en knapp för att initiera. \n\r";
    char msg1[] = "\n\r Hur många gånger ska programmet loopa? \n\r";
    char msg2[] = "\n\r Vilken delay (ms) ska det vara mellan blinkningarna? \n\r";
    //Skicka meddelande
    uarte_write(msg, strlen(msg));
    //Mainloop med funktioner
    while(1)
    {
        //Delay så programmet inte kör för snabbt
        delay();
        //Kolla status
        status1 = button_read(BUTTON1);
        //Kolla om en knapp blir trycks NER
        if(status0 == PIN_HIGH && status1 == PIN_LOW)
        {
            //Initiera rand med RTC-tiden som seed
            srand(nrfx_rtc_counter_get(&rtc_instance));

            uarte_write(msg1, strlen(msg1));
            loopAmount = read_int();
            uarte_write(msg2, strlen(msg2));
            delayTime= read_int();
            for(int i = 0; i<loopAmount; i++)
            {
                //Tänd random lampa
                tempLight = rand()%4+28;
                led_write(tempLight, LED_ON);
                nrfx_systick_delay_ms(delayTime);
                led_write(tempLight, LED_OFF);
            }
            sprintf(str, "\n\r%d\n\r", delayTime*loopAmount);
            uarte_write(str, strlen(str));
        }
        //Knapp status är up-to-date
        status0 = status1;
    }
}