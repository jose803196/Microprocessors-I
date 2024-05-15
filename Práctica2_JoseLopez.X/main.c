/* 
 * File:   main.c
 * Author: Jose G. Lopez
 * C.I. V-26.893.008
 * Created on 17 de abril de 2024, 08:11 AM
 * Modified on 13 de mayo de 2024, 17:33 PM
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
const unsigned char tab1[10]={1,2,3,4,5,6,7,8,9,0};
long Suma (int, int);
void calc(int *var,int *var2,int,int);

int x;


#define _XTAL_FREQ 48000000    //defino frecuencia del oscilador para que el macro delay defina el retardo
__attribute__((address(0x1006))) const int firma[] = {0x600d}; //incluyo firma para que el bootloadre reconazca el programa de usuario


int main(int argc, char** argv) {
    
    volatile unsigned int o=0x5555,q=0;
    
    char l=0;
    
    for(l=0;l<=5;l++){
        
        q=o>>1;
    }
    
    
    config();
   char tab2[MAXVAL];
   char n;
   
   for (n=0;n<=9;n++){
       
       tab2[n]=n+5;
       
   }
   
    
    /* estructuras */

    persona sujeto1;
    persona sujeto2;
    sujeto2.altura=180;
    
    
    sujeto1.altura=160;
    sujeto1.edad=20;
    sujeto1.sexo=1;
    strcpy (sujeto1.nombre,"Pedro");
    
/* ejemplo de memcmp*/
    
   volatile int comp=0;
   volatile char arreglo1[20];
   volatile char arreglo2[20]; 
   strcpy(arreglo1,"Earlos");
   strcpy(arreglo2,"Carla");
  
   comp=memcmp(arreglo1,arreglo2,3);

 /*   punteros          */   

volatile  int* p;
p=1;
volatile int k;
k=&p;
*p=8;
(*p)++;
*p=5;

/* pasar parametro por referencia */

int var_sum=0;
int var_mul=0;



calc(&var_sum,&var_mul,9,8);



    long int a = 0;
    int b = 0;
    int c = 0;
   
   volatile long d =0 ;
   x=5;
    a = 5123;
    b = 0x39;
    c = a + b;
    d=Suma (a,b);
    
    if (d==10)
    {
        d=0;   
    }
    else
    {
        d=1;
    }
    
    for (a=0;a<10;a=a+2)
    {
        d++;
    }
 a=5;       
while (a>2)
{
    a--;
}
 
 /* ejemplo de puertos */
 
 
 
while (1){
    
    
//    if(PORTBbits.RB0){
//        
//        LATAbits.LA1=0;
//    }
//    else{
//        
//        LATAbits.LA1=1;
//    }
    
    while(PORTBbits.RB0){
        LATAbits.LATA0=0;
        __delay_ms(500);
        LATAbits.LATA0=1;
        __delay_ms(500);
        
        LATAbits.LATA1=0;
        __delay_ms(250);
        LATAbits.LATA1=1;
        __delay_ms(500);
        
        LATAbits.LATA2=0;
        __delay_ms(500);
        LATAbits.LATA2=1;
        __delay_ms(150);
        
        LATAbits.LATA3=0;
        __delay_ms(1000);
        LATAbits.LATA3=1;
        __delay_ms(500);
    }
    
//    LATAbits.LATA0=0;
//    __delay_ms(1000);
//    while(! PORTBbits.RB0)
//    {
//        LATAbits.LATA0=1;
//    }
//    __delay_ms(10);
//    while(PORTBbits.RB0);
//    __delay_ms(100);
    
}   
return (EXIT_SUCCESS);
}

long Suma(int f ,int b)
{
 volatile int a=0;
    long resultado=0;
resultado=f+b;
 a=300;
 x=100;
    return resultado;
}
void calc(int *res_sum, int *res_mul,int a,int b){
    
    (*res_sum)=a+b;
    *res_mul=a*b;
}
