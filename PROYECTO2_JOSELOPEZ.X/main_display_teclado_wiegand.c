/*
 * File:    Proyecto 2
 * Author:  Jose Lopez
 * C.I.:    26.893.008
 *
 * Created on 30 de junio de 2024, 23:20
 */


#include <xc.h>
#include<string.h>
#include<stdio.h>
#include "config.h"
#include "wiegand.h"
#define _XTAL_FREQ 48000000    //defino frecuencia del oscilador para que el macro delay defina el retardo/*

__attribute__((address(0x1006))) const int firma[] = {0x600d}; //incluyo firma para que el bootloadre reconazca el programa de usuario
void tx_232(char *); //prototipo de la funcion tx_232

const char segmentos[]={0x5f,0x41,0x9b,0xd3,0xc5,0xd6,0xde,0x43,0xdf,0xd7,0xcf,0xdc,0x1e,0xd9,0x9e,0x8e};//0xc7
char codigo[5];
int estado;
int contador_de_lectura = 0;
int contador_de_carga = 0;
int aux_de_carga_inicial;
char byte_de_verif;
char b2,b1,b0;
char static rota;


const char mens1[]= "Pulsador RB0 activado \n";

char d1,d2,d3,d4;
char tecla;

void weeprom(char, char);
char reeprom(char);
void shift(char);
void verificacion_ini();
void dis_abierto();
void dis_cerrado();
void carga_ini();
void verificacion();
void lectura_tarjeta();
void permutacion_de_estados(int);


//*****  funcion de ervicio de la interrrupcion de alta prioridad **********
void __interrupt(high_priority) hint(void)
{   char rx_buf;
    //******* seccion para interfaz wiegand *********************************************
    if(INTCON3bits.INT1F){ // verifico interrupcion externa en pin RB1
           
        wuno();
            
            INTCON3bits.INT1F=0; //reactivo interrucion
            
        }
    //************************************************************************************
    if(INTCON3bits.INT2F){ // verifico interrupcion externa en pin RB2
            
        wcero();
            INTCON3bits.INT2F=0; //reactivo interrucion
        }
    //*************************************************************************************
       
    if(PIR1bits.RCIF){// verifico interrupcion por recepcion de puerto serial
        
             
            rx_buf=RCREG1;      //leo el byte recibido
            if(RCSTA1bits.OERR){ //verifio si hay error de overun y borro bandera de error
                RCSTAbits.CREN=0;
                RCSTAbits.CREN=1;
            }
            
            TXREG1=rx_buf;  //reenvio el byte leido 
            PIR1bits.TXIF=0; //limpio bandera de transmision
            while(!PIR1bits.TXIF); //espero a que se envie el byte
            
            if(contador_de_lectura <= 5){
                codigo[contador_de_lectura] = rx_buf;
                contador_de_lectura++;
            }
            
            PIR1bits.RCIF=0;  //reactivo la interrupcion
        }
return;
}

//*****  funcion de ervicio de la interrrupcion de baja prioridad **********
void __interrupt(low_priority) lint(void)
{       static unsigned int pulso=0;
        //************ rotacion de leds ******************
        pulso++;
        
        if(pulso==1000) pulso=0;
        
        
        switch(pulso){
            case 0:
                LED1=0; //encienmdo led 0
                LED2=1;
                LED3=1;
                LED4=1;
                
                break;
                
            case 25:
                LED1=1; 
                LED2=0; //encienmdo led 2
                LED3=1;
                LED4=1;
                
                
                break;
            case 50:
                LED1=1; 
                LED2=1;
                LED3=1;
                LED4=0; //encienmdo led 4
                
                
                break;
            case 75:
                LED1=1; 
                LED2=1;
                LED3=0; //encienmdo led 3
                LED4=1;
                
                break;   
        }
            INTCONbits.TMR0IF=0;    //reactivo interrupcion
   return;
}


void main(void) {
    config(); // configuro puertos, interrupciones, oscilador y perifericos
    conf_wig();
    char retorno=0x41;
    
    retorno=reeprom(0x01);
    tx_232(&retorno);
    retorno++;
    weeprom(0x1,retorno);
    
    BUZZER=1; //apago el buzzer
    tx_232("Bienvenidos \n");
    tx_232("Verificando sistema \n");
    
    
    verificacion_ini();
    
    if(aux_de_carga_inicial==1){
        carga_ini();
    }
    
    while(1){
        verificacion();
        lectura_tarjeta();    
    }
    
    while(1);
    
    return;
}

//****  Funcion que envia una cadena de caracteres por el puerto serial ********
void tx_232(char *mensaje){
    
    while (*mensaje != 0){
        
        TXREG1=*mensaje; //envio el caracter apuntado por el puntero
        PIR1bits.TXIF=0; //limpio bandera de transmision de dato
        while(!PIR1bits.TXIF); //espero a que se envie el byte
        
        mensaje++;
    }
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

void verificacion_ini(){
    if(reeprom(0x01)==0xff && reeprom(0x04)==0xff){
        estado=1;         
        aux_de_carga_inicial=1;
        dis_abierto();      
        tx_232("Ok, abre \n");  
    }
    else {             
        estado=0;
        aux_de_carga_inicial=0;
        dis_cerrado();      
        tx_232("Ok, cierra \n");  
    }
    return;
}

void dis_abierto(){
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
                shift(0xdc);
                
                break;
                
            case 2://display 2
                disp4=1;
                disp3=1;
                disp2=0;
                disp1=1;
                shift(0x1e);
                
                
                break;
            case 3://display 3
                disp4=1;
                disp3=0;
                disp2=1;
                disp1=1;
                
                shift(0x41);
                
                break;
            case 4://display 4
                disp4=0;
                disp3=1;
                disp2=1;
                disp1=1;
                
                shift(0x9e);
                
                break;
        }
    }
}

void dis_cerrado(){  
    disp1=segmentos[12];
    disp2=segmentos[11];
    disp3=segmentos[11];
    disp4=segmentos[15];
    PORTBbits.RB4 = 1;    
}

 void carga_ini(){
    tx_232("Cargar los codigos: \n");
    
    while(contador_de_carga<2){
    verificacion();
    }
    
    tx_232("Codigos cargados correctamente \n");
    tx_232("Se va a cerrar el sistema \n");
    dis_cerrado();
    estado=0;
    aux_de_carga_inicial=0;
    
    return; 
}

void verificacion(){
     
     if(contador_de_lectura>=5){     //si ya se recibieron 5 datos por el puerto
            contador_de_lectura=0;
            
            byte_de_verif=(codigo[0]+codigo[1]+codigo[2]+codigo[3])&0xFF;

            if(byte_de_verif==codigo[5] && estado==1 && codigo[0]== 01){     //veo si la suma es igual al byte chk        
                weeprom(codigo[0],codigo[1]);            //almaceno dato en la EEPROM
                weeprom(codigo[0]+0x01,codigo[2]);
                weeprom(codigo[0]+0x02,codigo[3]);
                tx_232("Ok \n"); //envío Ok por el puerto para confirmar dato correcto
                contador_de_carga++;     
            }
            else{
                if(byte_de_verif==codigo[5] && estado==1 && codigo[0]== 02){     //veo si la suma es igual al byte chk        
                    weeprom(codigo[0]+0x02,codigo[1]);            //almaceno dato en la EEPROM
                    weeprom(codigo[0]+0x03,codigo[2]);
                    weeprom(codigo[0]+0x04,codigo[3]);
                    tx_232("Dato Correcto.\n");  //envío Ok por el puerto para confirmar dato correcto
                    contador_de_carga++;
                }
                else{ 
                    tx_232("\nError\n"); //envío Error por el puerto para informar de dato incorrecto
                    if (estado==1){
                        tx_232("Valor incorrecto.\n");
                    }
                    if (estado==0){
                        tx_232("Sistema cerrado no se puede cargar codigo.\n");
                    }
                }
           }
     }
     
     return;
 }

void lectura_tarjeta(){
 
    if(wiegand (&b2,&b1,&b0)){
         
        if ((reeprom(0x01)==b2 && reeprom(0x02)==b1 && reeprom(0x03)==b0) || (reeprom(0x04)==b2 && reeprom(0x05)==b1 && reeprom(0x06)==b0)){
            permutacion_de_estados(estado);
        } 
    }
    return;
}

void permutacion_de_estados(int estados){
    if(estados==0){
        estado=1;
        dis_abierto();
        tx_232("Abriendo  sistema \n");
    }else{
        estado=0;
        dis_cerrado();
        tx_232("Cerrando  sistema \n");
    }
    return;
}