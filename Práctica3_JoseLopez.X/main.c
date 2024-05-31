#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h" // Asegúrate de que config.h contenga las configuraciones necesarias para tu microcontrolador

#define _XTAL_FREQ 48000000    // Defino la frecuencia del oscilador para el macro delay y otras funciones de tiempo

__attribute__((address(0x1006))) const int firma[] = {0x600d}; // Firma para que el bootloader reconozca el programa de usuario
volatile unsigned int t;
int cont_led, cont_led1, cont_led2, cont_led3;

void custom_delay_ms(unsigned int ms) {
    for(unsigned int i = 0; i < ms; i++) {
        __delay_ms(1);
    }
}

void __interrupt(low_priority) lowInterrupt(void) {
    if (PIR1bits.TMR1IF) {
        PIR1bits.TMR1IF = 0; // Limpiar bandera de interrupción
    }
}

void __interrupt(high_priority) highInterrupt(void) {
    if (INTCONbits.INT0F) {
        t += 100;
        if (t >= 1000) {
            t = 100;
        }
        INTCONbits.INT0F = 0; // Reactiva la interrupción
    }
}

int main(int argc, char** argv) {
    config(); // Configurar pines y registros
    
    // Configuro el temporizador para generar base de tiempo de 5ms
    T1CON = 0b00000000; // Modo temporizador, Prescaler 1:1
    TMR1 = 3036; // Valor para generar interrupción cada 5ms
    PIR1bits.TMR1IF = 0; // Limpiar bandera de interrupción
    PIE1bits.TMR1IE = 1; // Habilitar interrupción por desborde de temporizador 1

    t = 100;

    while (1) {
        custom_delay_ms(t);

        // Control de los LEDs
        if (cont_led == 1) {
            LATAbits.LATA0 = 1; // Encender LED 1
        }
        if (cont_led == 2) {
            LATAbits.LATA0 = 0; // Apagar LED 1
            cont_led = 0;
        }

        if (cont_led1 == 2) {
            LATAbits.LATA1 = 1; // Encender LED 2
        }
        if (cont_led1 == 4) {
            LATAbits.LATA1 = 0; // Apagar LED 2
            cont_led1 = 0;
        }

        if (cont_led2 == 4) {
            LATAbits.LATA2 = 1; // Encender LED 3
        }
        if (cont_led2 == 8) {
            LATAbits.LATA2 = 0; // Apagar LED 3
            cont_led2 = 0;
        }

        if (cont_led3 == 8) {
            LATAbits.LATA3 = 1; // Encender LED 4
        }
        if (cont_led3 == 16) {
            LATAbits.LATA3 = 0; // Apagar LED 4
            cont_led3 = 0;
        }

        cont_led++;
        cont_led1++;
        cont_led2++;
        cont_led3++;
    }

    return (EXIT_SUCCESS);
}