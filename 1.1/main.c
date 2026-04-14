#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#define MAXSIZE 12

//definierar vilka pinnar som ska användas för att skicka och ta emot data:
#define PIN_TXD 20
#define PIN_RXD 22

//Skapar en driver instance för UARTE:
nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);

//en funktion för att skicka strängen data av storlek length, över UARTE
void uarte_write(char* data, int length)
{
    nrfx_uarte_tx(&instance, data, length, 0);
}


void read_string(char *inArray)
{
    int i = 0;
    char c1;

    char msg1[] = " \n\r Skriv tecken så läses dem av och sedan återvänder dem vid 'Enter' eller efter max antal tecken! \n\r";
    uarte_write(msg1, sizeof(msg1));

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

int main(void)
{
    //Vi kör med default settings och anger våra TX- och RX-pinnar:
    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);

    //Initierar UARTE:
    nrfx_err_t errr = nrfx_uarte_init(&instance, &config, NULL);
    if (errr != 0){
    //Här kan vi implementera felhantering, men för stunden håller vi tummarna att allt funkar
    }
    char teckenArray[MAXSIZE+1];
    read_string(teckenArray);
}