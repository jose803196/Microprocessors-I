/* * File:   teclado_display_main.c
 * Author: Jose Lopez
 * C.I. 26.893.008
 *
 * Created on 23 de junio de 2024, 10:00 AM
 */

#include <xc.h>          // Encabezado para el microcontrolador específico (ajustar según el modelo)  
#include <stdint.h>      // Para definiciones de enteros  
#include <stdbool.h>     // Para tipos booleanos  
#include <string.h>      // Para funciones de manejo de cadenas  
#include <stdio.h>       // Para funciones de entrada/salida  

#define _XTAL_FREQ 48000000    //defino frecuencia del oscilador para que el macro delay defina el retardo/*

__attribute__((address(0x1006))) const int firma[] = {0x600d}; //incluyo firma para que el bootloadre reconazca el programa de usuario

const char segmentos[]={0x5f,0x41,0x9b,0xd3,0xc5,0xd6,0xde,0x43,0xdf,0xd7,0xcf,0xdc,0x1e,0xd9,0x9e,0x8e};
unsigned char sinetable[32]={0,0,1,3,5,7,10,13, 16,19,22,25,27,28,30,31,31,31,30,28,27,25,22,19,16,13,10,7,5,3,1,0};
const char c1[]={1,4,7,0x0e};
const char c2[]={2,5,8,0};
const char c3[]={3,6,9,0xf};
const char c4[]={0x0a,0xb,0x0c,0xd};
#include "config.h"
#include "wiegand.h"
#define tm0_h 0xd1;
#define tm0_l 0x1f;


char d1,d2,d3,d4;
char tecla;
char arreglo[4];
void shift(char);
char det_fila(void);
void tx_232(char *p,char); //prototipo de la funcion tx_232 
void Envia_cadena (const char* texto);  //funcion devuelve una cadena de caracteres
void Envia_frecuencia(int tiempo);      //funcion devuelve tiempo


char contadorserial=0, contador125=0,contador250=0,contador500=0, contadorp;
char frecuencia_125[]={70,61,48,49,50,53}; // F=0125 en codigo ascii
char frecuencia_250[]={70,61,48,50,53,48}; // F=0250 en codigo ascii
char frecuencia_500[]={70,61,48,53,48,48}; // F=0500 en codigo ascii
char frecuencia_preg[]={70,61,63};          // F=? en codigo ascii
int frec_val;

void __interrupt(high_priority) hint(void){
        char rx_buf; 
        int i = 0;
        int j = 0;
        if(PIR1bits.RCIF){ // verifico interrupcion por recepcion de puerto serial
            rx_buf=RCREG1;      //leo el byte recibido  
            if(RCSTA1bits.OERR){ //verifio si hay error de overun y borro bandera de error
                RCSTAbits.CREN=0;
                RCSTAbits.CREN=1;
            }
            contadorserial++;
            if (rx_buf==(frecuencia_125[contadorserial-1])){
                contador125++;  // Si el valor de la cadena recibida coincide con la cadena establecida aumenta el contador
            }
            else{
               contador125=0;    // caso contrario vuelve a 0.
            }
            if (rx_buf==(frecuencia_250[contadorserial-1])){
                contador250++;    // Igual que el anterior y para el resto de casos.
            }
            else{
               contador250=0;
            }
            if (rx_buf==(frecuencia_500[contadorserial-1])){
                contador500++;
            }
            else{
               contador500=0;
            }
            if (rx_buf==(frecuencia_preg[contadorserial-1])){
                contadorp++;
            }
            else{
                contadorp=0;
            }
            if ( (!(rx_buf==(frecuencia_preg[contadorserial-1]))) && (!(rx_buf==(frecuencia_125[contadorserial-1])))&& (!(rx_buf==(frecuencia_250[contadorserial-1])))&& (!(rx_buf==(frecuencia_500[contadorserial-1])))){
                contadorserial=0;  // Si alguna comparacion es erronea se indica que hay un error ya que no 
                contador125=0; // coincide con ningun caso pedido.
                contador250=0;
                contador500=0;
                contadorp=0;
                char *error = " Error intente de nuevo "; 
                Envia_cadena (error); //Envia mensaje "Error intente de nuevo"
            }
            if (contadorp==3){   // contadorp si llega a 3 significa que se recibio F=? o 3 caracteres iguales.
                contadorserial=0;  // reinicio de contadores
                contador125=0; 
                contador250=0;
                contador500=0;
                contadorp=0;
                char *textot = " Frecuencia actual "; 
                char *textott = " Hz "; 
                Envia_cadena (textot); //Envia mensaje "Frecuencia actual"
                Envia_frecuencia (frec_val); //Envia el valor de la frecuencia
                Envia_cadena (textott); //Envia mensaje "Hz"
            }
            if (contador125==6){ // Contador125 si llega a 6 significa que se recibio F=0125 o 6 caracteres iguales.
                contadorserial=0;  // reinicio de contadores
                contador125=0; 
                contador250=0;
                contador500=0;
                contadorp=0;
                frec_val=125; // como se recibio F=0125 establezco valor de la frecuencia en 125.
                char *textot = "Ok Frecuencia actualizada a "; 
                char *textott = " Hz "; 
                Envia_cadena (textot); //Envia mensaje "Frecuencia actualizada a"
                Envia_frecuencia (frec_val); //Envia el valor de la frecuencia
                Envia_cadena (textott); //Envia mensaje "Hz"
                d1=segmentos[0];
                d2=segmentos[1];
                d3=segmentos[2];
                d4=segmentos[5];             
            }
            if (contador250==6){  // Igual para todos los casos
                contadorserial=0;  // reinicio de contadores
                contador125=0; 
                contador250=0;
                contador500=0;
                contadorp=0;
                frec_val=250; // como se recibio F=0250 establezco valor de la frecuencia en 250.
                char *textot = "Ok Frecuencia actualizada a "; 
                char *textott = " Hz "; 
                Envia_cadena (textot); //Envia mensaje "Frecuencia actualizada a"
                Envia_frecuencia (frec_val); //Envia el valor de la frecuencia
                Envia_cadena (textott); //Envia mensaje "Hz"
                d1=segmentos[0];
                d2=segmentos[2];
                d3=segmentos[5];
                d4=segmentos[0];
            }
            if (contador500==6){
                contadorserial=0;  // reinicio de contadores
                contador125=0; 
                contador250=0;
                contador500=0;
                contadorp=0;
                frec_val=500; // como se recibio F=0500 establezco valor de la frecuencia en 500.
                char *textot = "Ok Frecuencia actualizada a "; 
                char *textott = " Hz ";           
                Envia_cadena (textot); //Envia mensaje "Frecuencia actualizada a"
                Envia_frecuencia (frec_val); //Envia el valor de la frecuencia
                Envia_cadena (textott); //Envia mensaje "Hz"
                d1=segmentos[0];
                d2=segmentos[5];
                d3=segmentos[0];
                d4=segmentos[0];
            }
        PIR1bits.RCIF=0;  //reactivo la interrupcion
        }
    return;
}

//*****  funcion de ervicio de la interrrupcion de baja prioridad **********
void __interrupt(low_priority) lint(void){      
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


void main(void){
    config(); // configuro puertos, interrupciones, oscilador y perifericos
    BUZZER=1; //Apago el Buzzer
    char contador_digitos;
    int i=0;

    tecla=0x5f;
    while(1){
        VREFCON2bits.DACR=sinetable[i];
        i++;
        __delay_us(62.5);           //Cambiar el delay cambia la frecuencia de salida.
        if(i == 33){
            i=0;
        }
        if(tecla!=0xff && tecla != 0x5f){
            if (tecla == 0x0A){ // Verifica si la tecla "A" est? presionada
                contador_digitos = 0;
                d1 = d3 = d4 = 0x5F; // Muestro el cero
                d2 = 0xd6;          //Muestro el cinco
                __delay_ms(200); //Retardo para eviar efecto rebote 
                BUZZER=0; //Enciendo Buzzer
                __delay_ms(50); //Espero 50 milisegundos
                BUZZER=1; //Apago el Buzzer
            }
            else{ // Actualiza los d?gitos en los displays si hay menos de 4 digitos en la pantalla y si se presiona una tecla v?lida.
                if (contador_digitos < 5 && tecla != 0x0b && tecla != 0x0c && tecla != 0x0d && tecla != 0x0e && tecla != 0x0f){
                    contador_digitos++;
                    d1 = d2; // Desplazar los d?gitos hacia la derecha
                    d2 = d3; // Desplazar los d?gitos hacia la derecha
                    d3 = d4; // Desplazar los d?gitos hacia la derecha
                    d4 = segmentos[tecla]; // Mostrar nuevo d?gito en el ?ltimo display?de?la?derecha
                    arreglo[contador_digitos-1] = tecla;
                    BUZZER=0; //Enciendo Buzzer 
                    __delay_ms(50); //Espero 50 milisegundos
                    BUZZER=1; //Apago el Buzzer
                    __delay_ms(200); //Retardo para evitar efecto rebote
                    
                    if ((d1)== 0x5F && (d2) == 0xd6 && (d3) == 0x5F && (d4) == 0x5F){
                        //Devolver la palabra error
                        d1 = 0x5F;
                        d2 = 0xd6; 
                        d3 = 0x5F; 
                        d4 = 0x5F;
                        tx_232("Ok frecuencia actualizada a 500Hz.",34); //Envio mensaje de "Ok frecuencia actualizada a 500Hz." 
                        while(1){
                        VREFCON2bits.DACR=sinetable[i];
                            i++;
                            __delay_us(62.5);           //Cambiar el delay cambia la frecuencia de salida.
                            if(i == 33){
                                i=0;
                            }
                            if (tecla == 0x0A)break;
                        }
                    }
                    if((d1)== 0x5F && (d2) == 0x9b && (d3) == 0xd6 && (d4) == 0x5F){
                        d1 = 0x5F;
                        d2 = 0x9b; 
                        d3 = 0xd6; 
                        d4 = 0x5F;
                        tx_232("Ok frecuencia actualizada a 250Hz.",34); //Envio mensaje de "Ok frecuencia actualizada a 250Hz."
                        while(1){
                        VREFCON2bits.DACR=sinetable[i];
                            i++;
                            __delay_us(125);           //Cambiar el delay cambia la frecuencia de salida.
                            if(i == 33){
                                i=0;
                            }
                            if (tecla == 0x0A)break;
                        } 
                    }
                    if((d1)== 0x5F && (d2) == 0x41 && (d3) == 0x9b && (d4) == 0xd6){
                        d1 = 0x5F;
                        d2 = 0x41; 
                        d3 = 0x9b; 
                        d4 = 0xd6;
                        tx_232("Ok frecuencia actualizada a 125Hz.",34); //Envio mensaje de "Ok frecuencia actualizada a 125Hz."
                        while(1){
                        VREFCON2bits.DACR=sinetable[i];
                            i++;
                            __delay_us(250);           //Cambiar el delay cambia la frecuencia de salida.
                            if(i == 33){
                                i=0;
                            }
                            if (tecla == 0x0A)break;
                        }
                    }
                    if(contador_digitos == 4){
                        d1 = 0x9e;
                        d2 = 0x88; 
                        d3 = 0x88; 
                        d4 = 0x7F;
                        tx_232("Valor de frecuencia incorrecto.",31); //Envio mensaje de "Ok frecuencia actualizada a 500Hz." 
                    }
                }
            }
            tecla=0xff;
        }
    } 
    return;
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

void tx_232(char *mens,char longitud){
    while (longitud>0){
        TXREG1=*mens; //envio el caracter apuntado por el puntero
        PIR1bits.TXIF=0; //limpio bandera de transmision de dato
        while(!PIR1bits.TXIF); //espero a que se envie el byte
        longitud--;
        mens++;
    }   
}

void Envia_cadena (const char *texto){ //Funcion para enviar mensajes por el puerto serial
    unsigned int cantidad = strlen(texto); //obtiene la cantidad de caracteres del arreglo
    char buffer[100]; //crea buffer de 100 elementos
    sprintf(buffer,"%s",texto); //guarda en el bufer la informacion de la variable tiempo en formato ascii
    for (int i=0;i<cantidad;i++){ //envia la informacion del buffer
        TXREG1=buffer[i];
        PIR1bits.TXIF=0; //limpio bandera de transmision
        while(!PIR1bits.TXIF); //espero a que se envie el byte
    }
}

void Envia_frecuencia (int frec_val){ //envia la frecuencia por el puerto serial
    int cant_digitos=0;
    int frecuencia1=frec_val;
    
    while (frecuencia1 > 0){ //obtiene la cantidad de digitos
        cant_digitos++;
        frecuencia1/=10 ; 
    }
    char buffer[10]; 
    sprintf(buffer,"%d",frec_val); //guarda en el bufer la informacion de la variable tiempo en formato ascii
    for (int i=0;i<cant_digitos;i++){ //envia la informacion del buffer
        TXREG1=buffer[i];
        PIR1bits.TXIF=0; //limpio bandera de transmision
        while(!PIR1bits.TXIF); //espero a que se envie el byte
    }
}