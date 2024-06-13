/* 
 * File:                config.h
 * Author:              Ivan Gutierrez
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  

#include <xc.h> // include processor files - each processor file is guarded.  

void config (void);  // prototipo de la funcion config()
void weeprom(char dir, char dato);
char reeprom(char dir);



#define tm0_h 0xd1;  // valores de recarga para el timer 0
//#define tm0_h 0x3c;  // valores de recarga para el timer 0
#define tm0_l 0x1f;  // 4 ms





//#define tm0_l 0xaf;  // 4 ms
//#define tm0_h 0x98;  // valores de recarga para el timer 0
//#define tm0_l 0x3a;  // 5 ms




//******** definicion de bits del puerto A ******

#define	segA	RA0	//Segmento A del display 7 seg. (salida dig.)
#define segB	Ra1	//Segmento B del display 7 seg. (salida dig.)
#define segE	RA2	//Segmento E del display 7 seg. (salida dig.)
#define segP	RA3 //Punto decimal del display 7 seg. (salida dig.)
#define segC	RA4 //Segmento C del display 7 seg. (salida dig.)
#define segF	RA5 //Segmento F del display 7 seg. (salida dig.)
#define segG	RA6	//Segmento G del display 7 seg. (salida dig.)
#define sefD	RA7 //Segmento D del display 7 seg. (salida dig.)

#define	tris_a	0x00	//configuración direcion de puerto a
#define ansel_a	0x00	//configuración modo puerto a



//******** definicion de bits del puerto B ******

#define	aux0	RB0	//salida auxiliar 0
#define	aux1	RB1	//salida auxiliar 1
#define	aux2	RB2	//salida auxiliar 2
#define	aux3	RB3	//salida auxiliar 3
#define	LCDE	PORTB,RB3	//enable LCD

#define	aux4	RB4	//salida auxiliar 4
#define	LCDRS	PORTB,RB4	//sel. modo LCD

//RB6 PGC
//RB7 PGD

#define	tris_b	0xE1	//configuración direcion de puerto b
#define ansel_b	0x00	//configuración modo puerto b



//******** definicion de bits del puerto C ******

#define	aux5	RC0	//salida auxiliar 5
#define cat4	LATCbits.LC1	//catodo 4 del display 7 seg. (salida dig.)
#define tecf4   LATCbits.LC1    //fila 4 del teclado
//RC2 no conectado
#define serTX	RC6	//salida comunicación serie (salida dig.)
#define serRX	RC7	//entrada comunicación serie (entrada dig.)


#define	tris_c	0xBC	//configuración direcion de puerto c
#define ansel_c	0x00	//configuración modo puerto c

//******** definicion de bits del puerto D ******

#define tecc1	RD0	//Col. 1 del teclado(entrada dig.)
#define tecc2	RD1	//Col. 2 del teclado(entrada dig.)
#define tecc3	RD2	//Col. 3 del teclado(entrada dig.)
#define tecc4	RD3	//Col. 4 del teclado(entrada dig.)

#define LCDD4	RD4	//Entrada datos LCD D4 (salida dig.)
#define LCDD5	RD5	//Entrada datos LCD D5(salida dig.)
#define LCDD6	RD6	//Entrada datos LCD D6(salida dig.)
#define LCDD7	RD7	//Entrada datos LCD D7(salida dig.)


#define	tris_d	0x0F	//configuración direcion de puerto d
#define ansel_d	0x00	//configuración modo puerto d


//******** definicion de bits del puerto E ******

#define tecf1	LATEbits.LE0	//Fila 1 del teclado (salida dig.)
#define tecf2	LATEbits.LE1	//Fila 2 del teclado (salida dig.)
#define tecf3	LATEbits.LE2	//Fila 3 del teclado (salida dig.)

#define	cat3	LATEbits.LE0	//Catodo 3 del display 7 seg.(salida dig.)
#define cat2	LATEbits.LE1	//Catodo 2 del display 7 seg.(salida dig.)
#define cat1    LATEbits.LE2	//Catodo 1 del display 7 seg.(salida dig.)




#define	tris_e	0x00	//configuración direcion de puerto d
#define ansel_e	0x00	//configuración modo puerto d	

