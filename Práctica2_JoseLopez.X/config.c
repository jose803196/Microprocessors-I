//***************************************************************************
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
   
    
}

