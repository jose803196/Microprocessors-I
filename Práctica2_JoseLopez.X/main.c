#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

#define MAXVAL 10
#define _XTAL_FREQ 48000000    //defino frecuencia del oscilador para que el macro delay defina el retardo
__attribute__((address(0x1006))) const int firma[] = {0x600d}; //incluyo firma para que el bootloadre reconazca el programa de usuario

//Creo una función que inicializa los leds y los pone en cero.
void init_leds(void) {
    LATAbits.LATA0 = 0; //LED 1 Apagado
    LATAbits.LATA1 = 0; //LED 2 Apagado
    LATAbits.LATA2 = 0; //LED 3 Apagado
    LATAbits.LATA3 = 0; //LED 4 Apagado
}

/*
 * Creo una función que recibe 2 variables led y estado
 * con la primera variable digo el led y con el segundo el estado
 */
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

//Creo la función de intermitencia
void intermitencia(void) {
    // Inicializo los 4 contadores, uno por LEDs
    static int cont_led1 = 0, cont_led2 = 0, cont_led3 = 0, cont_led4 = 0;
    //Si el contador de led 1 llega a 50 enciende
    if (cont_led1 == 50) {
        set_led(0, 1);
    }
    //Si el contador de led 1 llega a 100 apaga
    if (cont_led1 == 100) {
        set_led(0, 0);
        cont_led1 = 0;
    }
    //Si el contador de led 2 llega a 50 enciende
    if (cont_led2 == 50) {
        set_led(1, 1);
    }
    //Si el contador de led 2 llega a 75 apaga
    if (cont_led2 == 75) {
        set_led(1, 0);
        cont_led2 = 0;
    }
    //Si el contador de led 3 llega a 15 enciende
    if (cont_led3 == 15) {
        set_led(2, 1);
    }
    //Si el contador de led 3 llega a 65 apaga
    if (cont_led3 == 65) {
        set_led(2, 0);
        cont_led3 = 0;
    }
    //Si el contador de led 4 llega a 50 enciende
    if (cont_led4 == 50) {
        set_led(3, 1);
    }
    //Si el contador de led 4 llega a 150 apaga
    if (cont_led4 == 150) {
        set_led(3, 0);
        cont_led4 = 0;
    }
    //Aumento todos los contadores en uno
    cont_led1++;
    cont_led2++;
    cont_led3++;
    cont_led4++;
    __delay_ms(10);//Agrego un delay de 10 ms
}

int main(void) {
    config();
    volatile char state = 0;

    init_leds();

    while (1) {
        switch (state) {
            // Creo casos: Intermitencia, encender y apgar led´s
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
        //Verifico que el boton este presionado o no.
        if (PORTBbits.RB0 == 0) {
            state = (state + 1) % 3; //Dejo que el estado solo quede entre valores de 0,1 y 2.
            __delay_ms(500);
        }
    }

    return 0;
}