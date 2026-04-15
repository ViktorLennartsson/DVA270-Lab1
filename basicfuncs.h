#ifndef BASIC_FUNCS_H
#define BASIC_FUNCS_H

//Definerar knapp-ID och status-värden
#define BUTTON1 23
#define BUTTON2 24
#define BUTTON3 8
#define BUTTON4 9
#define PIN_HIGH 1
#define PIN_LOW 0

//Definerar LED-ID och status-värden
#define LED1 28
#define LED2 29
#define LED3 30
#define LED4 31
#define LED_OFF 1
#define LED_ON 0

//Funktioner
void uarte_write(char* data, int length);
void read_string(char* inArray, int maxLen);
int read_int(int maxLen);
void send_int(int inputNumber);
void init_stuff(void);
void led_write(int LED_ID, int STATE);
int button_read(int buttonNumber);
void delay_s(int time);
void delay_ms(int time);
#endif