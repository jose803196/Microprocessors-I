/*
 * File: main
 * Autor: Jose Lopez
 * C.I. 26.893.008
 * Fecha: 15/06/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "config.h"

#define MAXVAL 10
#define _XTAL_FREQ 48000000  // Defino frecuencia del oscilador para que el macro delay defina el retardo
#define TMR0H_5ms 0x8A
#define TMR0L_5ms 0xB0

__attribute__((address(0x1006))) const int firma[] = {0x600d}; // Incluyo firma para que el bootloader reconozca el programa de usuario
volatile int t_ms = 100; // Defino tiempo inicial
char arreglo[12];
unsigned char i = 0;
const char segmentos[]={0x5f,0x41,0x9b,0xd3,0xc5,0xd6,0xde,0x43,0xdf,0xd7,0xcf,0xdc,0x1e,0xd9,0x9e,0x8e};
// 65, 155, 211, 197, 214, 222, 67, 223, 215, 95, 119, 244, 165, 214,229, 101, arreglo de arriba en decimal
// 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F

void buf_clean();
void netw_procss();
void netw_trans(const char* mensaje);

/*      Funcion para cargar dato en  el shift register      */
void shift(char dato){
    // inicio reset
    SDATA=1;
    MR=1;
    
    //genero pulso de reset
    MR=0;
    MR=1;
    
    // inicio se ales de reloj
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


/*      Servicio de interrupción de alta prioridad          */
void __interrupt(high_priority) hint(void){
    if (PIR1bits.RCIF){
        char leer = RCREG1;// Leo el byte recibido

        if (RCSTA1bits.OERR){// Verifico si hay errores
            RCSTA1bits.CREN = 0; // Deshabilito y habilito la recepción para limpiar el error
            RCSTA1bits.CREN = 1;
            PIR1bits.RCIF = 0; // Limpio la bandera de recepción para evitar procesar datos incorrectos
            i = 0; // Reinicio índice en caso de error
            buf_clean(); // Limpio el buffer
            return;
        }

        if (i < sizeof(arreglo) - 1){// Almaceno el byte recibido en el buffer si hay espacio
            arreglo[i++] = leer;
            arreglo[i] = '\0';
        }

        if (leer == '=' || i >= 7){// Proceso la cadena si es completa o si recibimos un carácter no válido para continuar
            if (strncmp((const char*)arreglo, "tiempo=?", 8) == 0 || i == 11){// Verifico si la cadena es "tiempo=?" o tiene la longitud esperada de 11 caracteres
                netw_trans("Recibido: ");
                netw_trans((const char*)arreglo);
                netw_trans("\r\n");
                netw_procss();
                i = 0; // Reiniciar el índice para la siguiente cadena
                buf_clean(); // Limpio el buffer después de procesar la cadena
            }
            else if (leer != 't' && leer != 'i' && leer != 'e' && leer != 'm' && leer != 'p' && leer != 'o' && leer != '=' && leer != '?' && !isdigit(leer)){
                netw_trans("error"); //Envio un mensaje de error
                i = 0;// Reinicio el índice para la siguiente cadena
                buf_clean(); // Limpio el buffer después de enviar el error
            }
        }
        PIR1bits.RCIF = 0;// Limpio la bandera de recepción
    }
return;    
}

/*      Servicio de interrupción de baja prioridad          */
void __interrupt(low_priority) lint(void)
{       
        static char rota;
        static unsigned int pulso=0;
        
        static int cont1; //Auxiliar para el tiempo
        static int cont2; 

        if(INTCONbits.TMR0IF){ // verifico interrupcion por TIMER 0
            
        rota++; //actializo contador de rotacion
        if(rota==5) rota=1;
        
        /*      Rotacion de digitos para el multiplexado        */
       
        cont1++;
        
        if(cont1 >= (t_ms / 5)){ //Cuenta tm/5 interrupciones y aumenta el numero en el display
           cont2++;
           cont1=0;
           if(cont2>9999){
               cont2=0;
           };
        };
        
        switch(rota){
            case 1:
                disp1=0; //enciendo display mil
                disp2=1;
                disp3=1;
                disp4=1;
                shift(segmentos[((cont2/1000)%10)]);
                break;
                
            case 2:
                
                disp1=1;
                disp2=0; //enciendo display 2
                disp3=1;
                disp4=1;
                shift(segmentos[((cont2/100)%10)]);
               
                break;
            case 3:
                
                disp1=1;
                disp2=1;
                disp3=0; //enciendo display 3
                disp4=1;
                
                shift(segmentos[((cont2/10)%10)]);
               
                break;
            case 4:
                
                disp1=1;
                disp2=1;
                disp3=1;
                disp4=0; //enciendo display 4
                shift(segmentos[(cont2%10)]);
                break;
        }
            TMR0H = TMR0H_5ms;  // Cargo constantes de retardo
            TMR0L = TMR0L_5ms;
            INTCONbits.TMR0IF=0;    //reactivo interrupcion  
        }
return;
}


/*      Función Principal       */
void main(void) {
    config(); // configuro puertos, interrupciones, oscilador y perifericos
    
    BUZZER=1; //Apago el buzzer, 0 queda encendido. 
    while(1){  
    }
    return;
}

/*      Limpio el buffer        */
void buf_clean()
{
    // Limpiar el buffer de la cadena
    memset(arreglo, 0, sizeof(arreglo)); //establezco el primer recuento bytes de dest en el valor 0.
}

/*      Verifico y valido el mensaje a enviar al micro      */
void netw_procss()
{
    // Validación explícita de formatos válidos
    if (strncmp((const char*)arreglo, "tiempo=", 7) == 0){
        if (strcmp((const char*)arreglo, "tiempo=?") == 0){
            // Enviar por UART el valor actual de t_ms
            char message[30];
            sprintf(message, "El tiempo actual es %d", t_ms);
            netw_trans(message);
        }
        else{

            int tiempo_nuevo = atoi((const char*)&arreglo[7]);// Obtengo el valor que me interesa, y poder pasarlo a la variable t_ms

            if (tiempo_nuevo >= 100 && tiempo_nuevo <= 1000 && tiempo_nuevo % 100 == 0){// Valido que el valor esté en el rango correcto y sea múltiplo de 100
                t_ms = tiempo_nuevo;
                char mensaje[30];
                sprintf(mensaje, "El nuevo tiempo es %d", t_ms); //Envio un mensaje del nuevo tiempo.
                netw_trans(mensaje);
            }
            else{
                netw_trans("error");
            }
        }
    }
    else{
        netw_trans("error"); //Si no son formatos validos mando un mensaje de error.
    }
}

/*      Transmito el mensaje        */
void netw_trans(const char* mensaje)
{
    while (*mensaje != '\0')
    {
        while (!TXSTAbits.TRMT); // Esperar a que el buffer de transmisión esté vacío
        TXREG = *mensaje++; // Transmitir el siguiente carácter
    }
}