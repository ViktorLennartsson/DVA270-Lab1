#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_systick.h>
#include <nrfx_uarte.h>

#define MAXSIZE 16
#define MAXWAIT 10

//definierar vilka pinnar som ska användas för att skicka och ta emot data:
#define PIN_TXD 20
#define PIN_RXD 22

//Skapar en driver instance för UARTE:
nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);

//en funktion för att skicka strängen data av storlek length, över UARTE
void uarte_write(char* data, int length)
{
	nrfx_uarte_tx(&instance, data, length, 0);  //Skriver ut till uarte
}


void read_string(char *inArray)
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
	} while((i<MAXSIZE) && (currentInput != '\r'));
	
	inArray[i]='\0';    //Lägger till \0 som sista tecken
	//uarte_write(inArray, i);  //Skriver ut inArray med storleken 1 gånger i tecken
}


int read_int(void)
{
	char teckenArray[MAXSIZE+1];    //Skapar char sträng
	read_string(teckenArray);   //Tar teckenArray som input och fyller den från från funktionen
	return atoi(teckenArray);   //lämnar tillbaka
}


void delay_s(int time)	//Delay
{
	nrfx_systick_delay_ms(time*1000);
}


int main(void)
{
    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);
	//Initierar UARTE:
	nrfx_err_t errr = nrfx_uarte_init(&instance, &config, NULL);
	if (errr != 0){
	//Här kan vi implementera felhantering, men för stunden håller vi tummarna att allt funkar
	}
    //Initierar systick:
    nrfx_systick_init();

	//Skapar variabler
    int waitTime;
	//Skapar meddelanden
    char msg1[] = " \n\r Skriv in ett heltal så kommer programmet vänta så länge innan det svarar\n\r";
    char msg2[] = " \n\r Det tar för lång tid!!! \n\r";
    char msg3[] = " \n\r Hej hej! \n\r";
	//Main loop med funktioner
    while(1)
    {
		//Skriver ut instruktion för programmet
        uarte_write(msg1, sizeof(msg1));
		//Läs väntetiden från användaren
        waitTime = read_int();
		//Kolla om väntetiden överskrids
        if(waitTime>MAXWAIT)
        {
			//Skriv att maxtiden överskrids
            uarte_write(msg2, sizeof(msg2));
        }
        else
        {
			//Vänta i väntetiden
            delay_s(waitTime);
			//Skriv att vi nu har väntat
            uarte_write(msg3, sizeof(msg3));
        }
    }
}   