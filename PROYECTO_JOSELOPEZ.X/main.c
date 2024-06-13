/* 
 * File:   main.c
 * Author: Jose Lopez
 * C.I. 26893008
 * 
 * Created on 03 de junio de 2024, 12:10 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

#define MAXVAL 10

#define _XTAL_FREQ 48000000    //defino frecuencia del oscilador para que el macro delay defina el retardo
__attribute__((address(0x1006))) const int firma[] = {0x600d}; //incluyo firma para que el bootloadre reconazca el programa de usuario
#define TMR0H_5ms 0x8A
#define TMR0L_5ms 0xB0


int contador;
int tiempo; 
int acumulador;
int num_int;
char cuenta;
char form_tiempo1[]={116,105,101,109,112,111,61}; 
char form_tiempo2[]={84,73,69,77,80,79,61}; 
char error[]={101,114,114,111,114}; //palabra error en codigo ascii
char listo[]={32,76,105,115,116,111,32,112,97,114,97,32,114,101,99,105,98,105,114}; //frase "listo para recibir" en codigo ascii
char datos_rec[]={32,68,97,116,111,115,32,114,101,99,105,98,105,100,111,115,32,115,105,110,32,101,114,114,111,114,32}; //Oracion "datos recibidos sin error" en codigo ascii 
char tiempo_actual[]={32,84,105,101,109,112,111,32,97,99,116,117,97,108,32}; //frase "Tiempo actual " en codigo Ascii 
char ms[]={32,109,115};
int carac_rx; 


void Envia_text (char texto[], int num_let);
void Envia_tiempo (int tiempo);
void Envia_cadena (const char* texto);


//*****  funcion de servicio de la interrrupcion de alta prioridad **********

void __interrupt(high_priority) hint(void)
{
     
    char    rx_buf;
    
    
    //*********************************************************************************    
    if(PIR1bits.RCIF){// verifico interrupcion por recepcion de puerto serial
        
             
            rx_buf=RCREG1;      //leo el byte recibido
            if(RCSTA1bits.OERR){ //verifico si hay error de overun  y borro bandera de error
                RCSTAbits.CREN=0;
                RCSTAbits.CREN=1;
            }
            carac_rx++;
             
            if (carac_rx<8) 
             {
                if ((rx_buf==(form_tiempo1[carac_rx-1])) || (rx_buf==(form_tiempo2[carac_rx-1])) )
                 {
                    acumulador=0; 
                 }
                else 
                 {
                    carac_rx=0;   
                    Envia_text(error,5); //envia la palabra error  
                    char* texto1 = " Error"; 
                    //Envia_cadena (texto1); //Envia el mensaje de error
                 }    
             }
               
                
            if (carac_rx==8)  
             {
                if (rx_buf ==63) 
                 {
                    //Envia_text(tiempo_actual,15); //envia "Tiempo actual" 
                    
                    
                    char* textot = " Tiempo actual "; 
                    Envia_cadena (textot); //Envia mensaje "tiempo actual"
                    Envia_tiempo (tiempo); //Envia el valor de la variable tiempo
                    
                    carac_rx=0; 
                 }
            
                else if ((rx_buf ==48) || (rx_buf ==49)) 
                  { 
                    int numero= rx_buf - '0';  
                    acumulador= numero*1000;  
                  }
                else 
                  {
                    carac_rx=0;  
                    //Envia_text(error,5); //envia la palabra error haciendo uso de la funcion
                    char* texto2 = " Error"; 
                    Envia_cadena (texto2); //Envia el mensaje de error 
                  } 
             }
                
            if (carac_rx==9) 
            {
             if ((acumulador==1000) && (!(rx_buf==48)))
               { 
                 carac_rx=0; 
                 //Envia_text(error,5); //envia la palabra error  
                 char* texto3 = " Error"; 
                 Envia_cadena (texto3); //Envia el mensaje de error 
               }
             if ((acumulador==0) && (!((rx_buf>48) && (rx_buf<=57))))
               {
                 carac_rx=0;  
                 //Envia_text(error,5); //envia la palabra error 
                 char* texto4 = " Error"; 
                 Envia_cadena (texto4); //Envia el mensaje de error 
               }
             else if ((acumulador==0) && ((rx_buf>48) && (rx_buf<=57)))
                {
                 int numero= rx_buf - '0';  
                 acumulador= acumulador + (numero*100);  
                }
            }
            
            
            if ((carac_rx==10)|| (carac_rx==11))  
             {
                if (rx_buf==48) 
                  {
                    if (carac_rx==11)  
                        {
                          //Envia_text(datos_rec,27); //envia mensaje de recepcion satisfactoria
                          char* texto5 = "Datos recibidos"; 
                          Envia_cadena (texto5); 
                          tiempo=acumulador; 
                          contador=0; 
                          carac_rx=0; 
                          
                        } 
                 }
                else 
                  {
                    carac_rx=0;  
                    //Envia_text(error,5); //envia la palabra error 
                    char* texto6 = " Error"; 
                    Envia_cadena (texto6); //Envia el mensaje de error 
                  }
             }
            
            PIR1bits.RCIF=0;  //reactivo la interrupcion
        }
   
   
return;
}



//*****  funcion de ervicio de la interrrupcion de baja prioridad **********
void __interrupt(low_priority) lint(void)
{   
        
        if(INTCONbits.TMR0IF){ // verifico interrupcion por TIMER 0
            //wig_tout();  //agrego para timeout de wiegand
            TMR0H=tm0_h;            //cargo constantes de retardo
            TMR0L=tm0_l;
     
            
        num_int = tiempo/5; 
        
        if(contador==num_int)
        {
            cuenta++;
            if (cuenta==16) cuenta=0; 
            LATA=cuenta;  
            contador=0;
        }
                  
        contador++; 
        INTCONbits.TMR0IF=0;   
        T0CONbits.TMR0ON = 1;              
        }

return;
}

int main(int argc, char** argv) {
    
    config();
    LATA=0;  
    contador=0; 
    cuenta=0; 
    tiempo=100;  
    acumulador=0; 
    carac_rx=0;  
    Envia_text(listo,19); //Envia la frase "Listo para recibir"
   
    while (1){   
 
    
    }   
return (EXIT_SUCCESS);
}


//Funcion para enviar mensajes

void Envia_text (char *texto, int num_let) //envia palabras almacenadas en arreglos de codigo ascii
{
    for (int i=0;i<num_let;i++) //envia la palabra error 
                      {
                        TXREG1=texto[i];
                        PIR1bits.TXIF=0; //limpio bandera de transmision
                        while(!PIR1bits.TXIF); //espero a que se envie el byte
                      }
}

void Envia_tiempo (int tiempo) //envia el tiempo por el puerto serial
{
    int cant_digitos=0;
    int tiempo1=tiempo;
    
    while (tiempo1 > 0) //obtiene la cantidad de digitos
     {
        cant_digitos++;
        tiempo1 /=10 ; 
     }
    char buffer[10]; 
    sprintf(buffer,"%d",tiempo); //guarda en el bufer la informacion de la variable tiempo en formato ascii
    for (int i=0;i<cant_digitos;i++) //envia la informacion del buffer
                      {
                        TXREG1=buffer[i];
                        PIR1bits.TXIF=0; //limpio bandera de transmision
                        while(!PIR1bits.TXIF); //espero a que se envie el byte
                      }
    
}

void Envia_cadena (const char* texto) //Funcion para enviar mensajes por el puerto serial
{
    unsigned int cantidad = strlen(texto); //obtiene la cantidad de caracteres del arreglo
    char buffer[100]; //crea buffer de 100 elementos
    sprintf(buffer,"%s",texto); //guarda en el bufer la informacion de la variable tiempo en formato ascii
    for (int i=0;i<cantidad;i++) //envia la informacion del buffer
                      {
                        TXREG1=buffer[i];
                        PIR1bits.TXIF=0; //limpio bandera de transmision
                        while(!PIR1bits.TXIF); //espero a que se envie el byte
                      }
}