/*
 * File:   teclado_display_main.c
 * Author: Jose Lopez
 * C.I. 26.893.008
 *
 * Created on 23 de junio de 2024, 10:00 AM
 */


#include <xc.h>
#include<string.h>
#include<stdio.h>
#define _XTAL_FREQ 48000000    //defino frecuencia del oscilador para que el macro delay defina el retardo/*

__attribute__((address(0x1006))) const int firma[] = {0x600d}; //incluyo firma para que el bootloadre reconazca el programa de usuario

const char segmentos[]={0x5f,0x41,0x9b,0xd3,0xc5,0xd6,0xde,0x43,0xdf,0xd7,0xcf,0xdc,0x1e,0xd9,0x9e,0x8e};
const char c1[]={1,4,7,0x0e};
const char c2[]={2,5,8,0};
const char c3[]={3,6,9,0xf};
const char c4[]={0x0a,0xb,0x0c,0xd};
#include "config.h"
#include "wiegand.h"


char d1,d2,d3,d4;
char tecla;
void shift(char) ;
char det_fila(void);


//*****  funcion de ervicio de la interrrupcion de alta prioridad **********

/*void __interrupt(high_priority) hint(void)
{
        if(tecla = 0x0A){// verifico interrupcion por recepcion de puerto serial
            
            
            tecla=0;  //reactivo la interrupcion
        }
return;
}*/

//*****  funcion de ervicio de la interrrupcion de baja prioridad **********
void __interrupt(low_priority) lint(void)
{      
        char fila=0;
        char static rota;
        static unsigned int pulso=0;
        if(INTCONbits.TMR0IF){ // verifico interrupcion por TIMER 0
            TMR0H=tm0_h;            //cargo constantes de retardo
            TMR0L=tm0_l;
            
        if (rota>=4)rota=1; //reiniciamos el contador de rotaciones
        else{
                rota++;
            }
            
        //************ rotacion de digitos ****************** 
        switch(rota){
            
            case 1: //display 1
                disp4=1;
                disp3=1;
                disp2=1;
                disp1=0;
                shift(d1);
                
                fila=det_fila();
                
                //verifico columna 1
                if(fila!=0) tecla=c1[fila-1];
                
                break;
                
            case 2://display 2
                disp4=1;
                disp3=1;
                disp2=0;
                disp1=1;
                shift(d2);
                
                fila=det_fila();
                 //verifico columna 2
                if(fila!=0)  tecla=c2[fila-1];
                
                break;
            case 3://display 3
                disp4=1;
                disp3=0;
                disp2=1;
                disp1=1;
                
                shift(d3);
                
                fila=det_fila();
                
                //verifico columna 3
                if(fila!=0)  tecla=c3[fila-1];
                break;
            case 4://display 4
                disp4=0;
                disp3=1;
                disp2=1;
                disp1=1;

                shift(d4);

                fila=det_fila();
                //verifico columna 4
                if(fila!=0) tecla=c4[fila-1];
                break;
        }
        INTCONbits.TMR0IF=0;    //reactivo interrupcion    
    }
return;
}


void main(void) {
    config(); // configuro puertos, interrupciones, oscilador y perifericos
    BUZZER=1; //Apago el Buzzer
    conf_wig();
    char contador_digitos;

    tecla=0x5f;
    while(1){
        if(tecla!=0xff && tecla != 0x5f){
            if (tecla == 0x0A) // Verifica si la tecla "A" está presionada
            {   
                contador_digitos = 0;
                d1 = d2 = d3 = d4 = 0x00; // Apagar los displays
                __delay_ms(200); //Retardo para eviar efecto rebote 
                BUZZER=0; //Enciendo Buzzer
                __delay_ms(50); //Espero 50 milisegundos
                BUZZER=1; //Apago el Buzzer
            }
            else // Actualiza los dígitos en los displays si hay menos de 4 digitos en la pantalla y si se presiona una tecla válida.
            {
                if (contador_digitos < 4 && tecla != 0x0b && tecla != 0x0c && tecla != 0x0d && tecla != 0x0e && tecla != 0x0f) 
                {   
                    contador_digitos++;
                    d1 = d2; // Desplazar los dígitos hacia la derecha
                    d2 = d3; // Desplazar los dígitos hacia la derecha
                    d3 = d4; // Desplazar los dígitos hacia la derecha
                    d4 = segmentos[tecla]; // Mostrar nuevo dígito en el último display de la derecha
                    BUZZER=0; //Enciendo Buzzer
                    __delay_ms(50); //Espero 50 milisegundos
                    BUZZER=1; //Apago el Buzzer
                    __delay_ms(200); //Retardo para evitar efecto rebote  
                }
            }
            tecla=0xff;
        }
    }   
    return;
}


// funcion que permite escribir un byte en la eeprom interna ///
void weeprom(char dir, char dato){ //dir=direccion del dato   dat=dato a escribir 1 byte
   
INTCONbits.GIE=0; //desactivo la interrupcion
EEADR= dir; //cargo la direccion del byte a escribir
EEDATA=dato;  //cargo el dato a escribir
 
 EECON1bits.WREN=1; //habilito escritura en eeprom

 EECON2=0x55; //genero codigo de seguridad para escribir en eeprom
 EECON2=0xAA;

 EECON1bits.WR=1; //activo la escritura en eeprom
 while(EECON1bits.WR==1); //esepro hasta que se escriba el byte
EECON1bits.WREN=0; //desactivo escritura en eeprom
INTCONbits.GIE=1; //reactivo las interrupciones
 //return;
}

//funcion que permite leer un byte de la eeprom interna
char reeprom(char dir){ //dir = direccion del byte a leer
    
    EEADR=dir; //cargo la direccion a leer
    EECON1bits.EEPGD=0; 
    EECON1bits.CFGS=0;
    EEDATA=0;
    EECON1bits.RD= 1; //activo lectura de eeprom
    return EEDATA; //retorno el byte leido
}

//*************** funcion para cargar dato en shift register ***********************

void shift(char dato){
    
    
    // inicio reset
    SDATA=1;
    MR=1;
    
    //genero pulso de reset
    MR=0;
    MR=1;
    
    // inicio senales de reloj
    PCLK=0;
    SCLK=0;
    
    
    char n=0,k=0;
    // lazo para enviar 8 bits
    for(n=0;n<=7;n++){
        
     if(dato&0x1)   SDATA=1; //preparo bit a enviar
     
     else SDATA=0;
     
     k=dato>>1; //roto dato a enviar
     dato=k; //actualizo dato rotado
      
    // genero pulso de reloj  
    SCLK=0;
    SCLK=1;
    SCLK=0;
   
   
          
    }
    //genero pulso de reloj paralelo
    PCLK=1;
    PCLK=0;
}

//funcion que permite detectar si hay una tecla pulsada y devuelve el numero de la fila
char det_fila(){
    volatile char fila=0;
    if((PORTD&0x01)==0){ //verifico si hay alguna tecla pulsada
        fila=1;
    }
    if((PORTD&0x02)==0){ //verifico si hay alguna tecla pulsada
        fila=2;
    }
    if((PORTD&0x04)==0){ //verifico si hay alguna tecla pulsada
        fila=3;
    }
    if((PORTD&0x08)==0){ //verifico si hay alguna tecla pulsada
        fila=4;
    }
    return fila;
}

