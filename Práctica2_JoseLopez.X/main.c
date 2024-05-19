#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

#define MAXVAL 10
#define _XTAL_FREQ 48000000    //defino frecuencia del oscilador para que el macro delay defina el retardo
__attribute__((address(0x1006))) const int firma[] = {0x600d}; //incluyo firma para que el bootloadre reconazca el programa de usuario

void init_leds(void) {
    LATAbits.LATA0 = 0;
    LATAbits.LATA1 = 0;
    LATAbits.LATA2 = 0;
    LATAbits.LATA3 = 0;
}

void set_led(int led, char state) {
    switch (led) {
        case 0:
            LATAbits.LATA0 = state;
            break;
        case 1:
            LATAbits.LATA1 = state;
            break;
        case 2:
            LATAbits.LATA2 = state;
            break;
        case 3:
            LATAbits.LATA3 = state;
            break;
    }
}

void intermitencia(void) {
    static int cont_led1 = 0, cont_led2 = 0, cont_led3 = 0, cont_led4 = 0;

    if (cont_led1 == 50) {
        set_led(0, 1);
    }
    if (cont_led1 == 100) {
        set_led(0, 0);
        cont_led1 = 0;
    }

    if (cont_led2 == 50) {
        set_led(1, 1);
    }
    if (cont_led2 == 75) {
        set_led(1, 0);
        cont_led2 = 0;
    }

    if (cont_led3 == 15) {
        set_led(2, 1);
    }
    if (cont_led3 == 65) {
        set_led(2, 0);
        cont_led3 = 0;
    }

    if (cont_led4 == 50) {
        set_led(3, 1);
    }
    if (cont_led4 == 150) {
        set_led(3, 0);
        cont_led4 = 0;
    }

    cont_led1++;
    cont_led2++;
    cont_led3++;
    cont_led4++;
    __delay_ms(10);
}

int main(void) {
    config();
    volatile char state = 0;

    init_leds();

    while (1) {
        switch (state) {
            case 0:
                intermitencia();
                break;
            case 1:
                set_led(0, 1);
                set_led(1, 1);
                set_led(2, 1);
                set_led(3, 1);
                break;
            case 2:
                set_led(0, 0);
                set_led(1, 0);
                set_led(2, 0);
                set_led(3, 0);
                break;
        }

        if (PORTBbits.RB0 == 0) {
            state = (state + 1) % 3;
            __delay_ms(500);
        }
    }

    return 0;
}