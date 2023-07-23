#include <xc.h>
#include <time.h>
#include <stdio.h> 
#include "newfile.h"
#include <adc.h>
#include <string.h>
#include <stdlib.h>


#define BUTON_MIN PORTAbits.RA4
#define BUTON_MAX PORTAbits.RA3
#define BUTON_RESET PORTAbits.RA2
#define MOTOR PORTBbits.RB12
#define RS PORTBbits.RB0
#define EN PORTBbits.RB1
#define D4 PORTBbits.RB2
#define D5 PORTBbits.RB3
#define D6 PORTBbits.RB4
#define D7 PORTBbits.RB5

int max = 0;
int min = 1000;
float ADCValue;
float tempinC;
char Buffer[2];
char Buffer_max[2];
char Buffer_min[2];
int i;


void delay(int number_of_seconds) {
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds)
        ;

}

void Lcd_SetBit(char data_bit) {
    if (data_bit & 1)
        D4 = 1;
    else
        D4 = 0;

    if (data_bit & 2)
        D5 = 1;
    else
        D5 = 0;

    if (data_bit & 4)
        D6 = 1;
    else
        D6 = 0;

    if (data_bit & 8)
        D7 = 1;
    else
        D7 = 0;
}

void Lcd_Cmd(char a) {
    RS = 0;
    Lcd_SetBit(a); //Incoming Hex value
    EN = 1;
    delay(4);
    EN = 0;
}

void Lcd_Clear() {
    Lcd_Cmd(0); //Clear the LCD
    Lcd_Cmd(1); //Move the curser to first position
}

void Lcd_Set_Cursor(char a, char b) {
    char temp;
    char z = 0;
    char y = 0;
    if (a == 1) {
        temp = 0x80 + b - 1; //80H is used to move the cursor
        z = temp >> 4; //Lower 8-bits
        y = temp & 0x0F; //Upper 8-bits
        Lcd_Cmd(z); //Set Row
        Lcd_Cmd(y); //Set Column
    } else if (a == 2) {
        temp = 0xC0 + b - 1;
        z = temp >> 4; //Lower 8-bits
        y = temp & 0x0F; //Upper 8-bits
        Lcd_Cmd(z); //Set Row
        Lcd_Cmd(y); //Set Column
    }
}

void Lcd_Start() {
    Lcd_SetBit(0x00);
    int i;
    for (i = 32767; i <= 0; i--) Nop();
    Lcd_Cmd(0x03);
    delay(5);
    Lcd_Cmd(0x03);
    delay(11);
    Lcd_Cmd(0x03);
    Lcd_Cmd(0x02); //02H is used for Return home -> Clears the RAM and initializes the LCD
    Lcd_Cmd(0x02); //02H is used for Return home -> Clears the RAM and initializes the LCD
    Lcd_Cmd(0x08); //Select Row 1
    Lcd_Cmd(0x00); //Clear Row 1 Display
    Lcd_Cmd(0x0C); //Select Row 2
    Lcd_Cmd(0x00); //Clear Row 2 Display
    Lcd_Cmd(0x06);
}

void Lcd_Print_Char(char data) //Send 8-bits through 4-bit mode
{
    char Lower_Nibble;
    char Upper_Nibble;
    Lower_Nibble = data & 0x0F;
    Upper_Nibble = data & 0xF0;
    RS = 1; // 
    Lcd_SetBit(Upper_Nibble >> 4); //Send upper half by shifting by 4
    EN = 1;
    int i;
    for (i = 65535; i <= 0; i--) Nop();
    EN = 0;
    Lcd_SetBit(Lower_Nibble); //Send Lower half
    EN = 1;
    for (i = 65535; i <= 0; i--) Nop();
    EN = 0;
}

void Lcd_Print_String(char *a) {
    int i;
    for (i = 0; a[i] != '\0'; i++)
        Lcd_Print_Char(a[i]); //Split the string using pointers and call the Char function 
}

char* itoa(int i, char b[]) {
    char const digit[] = "0123456789";
    char* p = b;
    if (i < 0) {
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do { //Move to where representation ends
        ++p;
        shifter = shifter / 10;
    } while (shifter);
    *p = '\0';
    do { //Move back, inserting digits as u go
        *--p = digit[i % 10];
        i = i / 10;
    } while (i);
    return b;
}

int main() {

    TRISA = 0;
    TRISB = 0;
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA3 = 1;
    TRISAbits.TRISA4 = 1;
    
    int temp;


    AD1CON1 = 0; // ASAM bit = 1 implies sampling ..
    AD1CHS0 = 0; // set channel 0 for sampling
    AD1PCFGL = 0xFFFE; // all pins set as digital except A0
    AD1CSSL = 0;
    AD1CON1 = 0x0E4;
    AD1CON2 = 0;
    AD1CON3 = 0x1F05; // Sample time manual, Tad = internal 2 Tcy
    AD1CON1bits.ADON = 1; // turn ADC ON

    Lcd_Start();
    while (1) {
        AD1CON1bits.SAMP = 0; // start Converting
        while (AD1CON1bits.DONE) // conversion done
        {
            ADCValue = ADC1BUF0; // get ADC value
            ADCValue = ADCValue * 4.88281; //convert it into voltage
            tempinC = ADCValue / 10; //getting the temperature values
            itoa(tempinC, Buffer);
        }
        
        temp = atoi(Buffer);

        if (temp < 23) {
            MOTOR = 0;
        }
        
        else {
            MOTOR = 1;
        }

        if (temp > max) {
            max = temp;
            itoa(max, Buffer_max);
        }
        
        if ((temp < min)&(temp < max)){
            min = temp;
            itoa(min, Buffer_min);
        }
        
        if (BUTON_RESET == 0) {
            max = 0;
            min = 1000;
            Lcd_Clear();
            Lcd_Set_Cursor(1, 2);
            Lcd_Print_String("Max resetat");
        }

        if (BUTON_MAX == 0) {
            Lcd_Clear();
            Lcd_Set_Cursor(1, 2);
            Lcd_Print_String("Temp max: ");
            Lcd_Set_Cursor(2, 4);
            for (i = 0; i <= 1; i++) {
                Lcd_Print_Char(Buffer_max[i]);
            }
            Lcd_Print_Char(0xDF);
        }

        if (BUTON_MIN == 0){
            Lcd_Clear();
            Lcd_Set_Cursor(1, 2);
            Lcd_Print_String("Temp min: ");
            Lcd_Set_Cursor(2, 4);
            for (i = 0; i <= 1; i++) {
                Lcd_Print_Char(Buffer_min[i]);
            }
            Lcd_Print_Char(0xDF);
        }
        
        Lcd_Clear();
        Lcd_Set_Cursor(1, 2);
        Lcd_Print_String("Temperatura");
        Lcd_Set_Cursor(2, 4);
        for (i = 0; i <= 1; i++) {
            Lcd_Print_Char(Buffer[i]);
        }
        Lcd_Print_Char(0xDF);
        Lcd_Print_Char(0x43);
        
       

    }

    return 0;
}





