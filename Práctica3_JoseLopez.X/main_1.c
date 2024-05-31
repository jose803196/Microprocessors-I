/* 
 * File:   main.c
 * Author: Ivan
 *
 * Created on 17 de abril de 2024, 08:11 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

#define MAXVAL 10
/*
 * 
 */

typedef struct {
        
    int edad;
    int altura;
    char sexo;
    char nombre[30];
    
} volatile persona;


int x;


#define _XTAL_FREQ 48000000    //defino frecuencia del oscilador para que el macro delay defina el retardo
__attribute__((address(0x1006))) const int firma[] = {0x600d}; //incluyo firma para que el bootloadre reconazca el programa de usuario
char estado;
 cont_led;

//*****  funcion de servicio de la interrrupcion de alta prioridad **********

void __interrupt(high_priority) hint(void)
{
    
    char    rx_buf;
     
    //*********************************************************************************
    if(INTCONbits.INT0F){ // verifico interrupcion externa en pin RB0
           
        
          LATAbits.LATA1=1;  
           estado++;
           if(estado==3)estado=0;
           __delay_ms(100);  
        
        INTCONbits.INT0F=0; //reactivo interrucion
            
        }
    
     if(PIR1bits.RCIF){// verifico interrupcion por recepcion de puerto serial
        
             
            rx_buf=RCREG1;      //leo el byte recibido
            if(RCSTA1bits.OERR){ //verifio si hay error de overun y borro bandera de error
                RCSTAbits.CREN=0;
                RCSTAbits.CREN=1;
            }
            rx_buf++;
            TXREG1=rx_buf;  //reenvio el byte leido 
            PIR1bits.TXIF=0; //limpio bandera de transmision
            while(!PIR1bits.TXIF); //espero a que se envie el byte
            
            PIR1bits.RCIF=0;  //reactivo la interrupcion
        }
   
return;
}
//*****  funcion de ervicio de la interrrupcion de baja prioridad **********
void __interrupt(low_priority) lint(void)
{   
        char static rota;

        if(INTCONbits.TMR0IF){ // verifico interrupcion por TIMER 0
            //wig_tout();  //agrego para timeout de wiegand
            TMR0H=tm0_h;            //cargo constantes de retardo
            TMR0L=tm0_l;
             //Pregunto por estado 0
    if(estado==0){
        
        
        if(cont_led==125){
            
            LATAbits.LATA0=1; //encedemos el led 
        }
        if(cont_led==250){
            
            LATAbits.LATA0=0; //apagamos el led 
            cont_led=0;
        }
        cont_led++;
        
    }
     //Pregunto por estado 1
    if(estado==1){
        
        LATAbits.LA0=1; //encendemos el led
    }
    //Pregunto por estado 2
    if(estado==2){
        
        LATAbits.LA0=0; //apagamos el led
    }
            
            
            INTCONbits.TMR0IF=0;    //reactivo interrupcion
           
    
             
        }

return;
}

int main(int argc, char** argv) {
    
    config();
    LATAbits.LATA1=0;

    
   
    estado=0;//estao inicial = 0
    cont_led=0;
    
 while (1){   
 
    
}   
return (EXIT_SUCCESS);
}

