#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 12

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
		

		//Om vi lyckades läsa någonting så skickar vi tillbaka det som togs emot
		uarte_write(&currentInput, 1);  //Skriver tillbaka så man ser vad man skriver
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

void send_int(int inputNumber)
{
	char outputBuffer[MAXSIZE+16];   //Skapar char sträng
	sprintf(outputBuffer, "%d \n\r", inputNumber);   //Konverterar inputNumber till string och lägger den i outputBuffer
	uarte_write(outputBuffer, strlen(outputBuffer));  //Skriver ut buffern med storlek av längden gånger 1 byte
}

int main(void)
{
	//Vi kör med default settings och anger våra TX- och RX-pinnar:
	const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);

	//Initierar UARTE:
	nrfx_err_t errr = nrfx_uarte_init(&instance, &config, NULL);
	if (errr != 0){
	//Här kan vi implementera felhantering, men för stunden håller vi tummarna att allt funkar
	}

	//1.1
	//char teckenArray[MAXSIZE+1];  //Skapar char sträng
	//read_string(teckenArray); //Tar teckenArray som input och fyller den från från funktionen
	//uarte_write(teckenArray, strlen(teckenArray)*1);  //Skriver ut värdet på teckenArray

    char msg1[] = " \n\r Skriv tecken så läses dem av och sedan återvänder dem vid 'Enter' eller efter max antal tecken!
    \n\r Samt konverteras till int och sedan tillbaka till text som du får!!! \n\r"; //Meddelande
    uarte_write(msg1, sizeof(msg1)); //Skicka meddelande

	//1.2
	int mainNumber;   //Skapar int
	mainNumber = read_int();  //Kör funktionen read_int och lägger return value i mainNumber

	//1.3
	//int mainNumber; //Skapar int
	send_int(mainNumber);   //Kör funktionen send_int med mainNumber som input

	//while(1);  //Ingen aning
}