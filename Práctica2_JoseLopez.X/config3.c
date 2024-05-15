/***************************************************************************
//*         Funcion de configuracion 
//*
//*         Ivan Gutierrez  16/02/2020
//*
//****************************************************************************

#include <xc.h>


#include "config.h"


void config(){
    
    // configuracion del oscilador
    
        OSCCON=0x70;	//ajusto el oscilador para trabajar a 48MHZ	
			
		OSCCON2=0x10;   //ajusto el oscilador para trabajar a 48MHZ	
		OSCTUNE=0x80;   //ajusto el oscilador interno para trabajar en la frecuencia de calibracion de fabrica	
        //while(!OSCCON2bits.PLLRDY) //espero que se ajute el osciladoe
    
    
    //****** configuracion de puertos paralelos
    
    TRISA=tris_a;
    TRISB=tris_b;
    TRISC=tris_c;
    TRISD=tris_d;
    TRISE=tris_e;
      
    ANSELA=ansel_a; 
    ANSELB=ansel_b; 
    ANSELC=ansel_c; 
    ANSELD=ansel_d; 
    ANSELE=ansel_e; 
    
    
    //configuracion de interrupciones
    
    INTCON=0;               // me aseguo de limpiar INTCON
    RCONbits.IPEN=1;        // activo opcion de interrupcion con prioridad
    INTCONbits.INT0E=1;     // habilito interrupcion externa en RB0
    INTCON2bits.INTEDG0=0;  // Indico interrupcion por falnco de bajada
    INTCONbits.GIEH=1;      // Activo interrupcion de alta prioridad
    INTCONbits.GIEL=1;      // Activo intrrupcion debaja prioridad
    INTCONbits.T0IE=1;      // Activo interrupcion de tmr0
    INTCON2bits.T0IP=0;     // defino interrupcion de baja prioridad para el tmr0
    PIE1bits.RCIE=1;        //activo interrupcion por recepcion de usart
    IPR1bits.RCIP=1;        //asigno prioridad alta para recepcion por puerto serie
    // configuracion del tmr0
    
    T0CONbits.T08BIT=0;     // funcionamiento en modo 16 bits
    T0CONbits.T0CS=0;       // selecciono fuente de reloj interna
    T0CONbits.PSA=0;        // seleciono uso del prescaler
    T0CONbits.T0PS=1;       // prescaler entre 4  
    
    //T0CON=0x80;
    
    TMR0H=tm0_h;            //cargo constantes de retardo
    TMR0L=tm0_l;
    
    // configuracion del usart
    
    SPBRG1 = 0x4d;          //velocidad de comunicacion 9600 baudios
    SPBRGH1 = 0;
    TXSTA1bits.BRGH=0;
    TXSTA1bits.TX9=0;       //transmision de 8 bits
    TXSTA1bits.TXEN=1;      //activo transmsion
    TXSTA1bits.SYNC=0;      //modo asincronico
    RCSTA1bits.RX9=0;       // modo de recepcion de 8 bits
    RCSTA1bits.CREN=1;      //habilito recepcion continua
    BAUDCON1=0x40;
    RCSTA1bits.SPEN=1;      //activo puerto serial
    
}

