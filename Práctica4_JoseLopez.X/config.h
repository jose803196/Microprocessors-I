

#include <xc.h> // include processor files - each processor file is guarded.  

 void config(void);

//******** definicion de bits del puerto A ******


#define	tris_a	0x00	//configuración direcion de puerto a
#define ansel_a	0x00	//configuración modo puerto a

//******** definicion de bits del puerto B ******

#
#define	tris_b	0x03	//configuración direcion de puerto b
#define ansel_b	0x00	//configuración modo puerto b

//******** definicion de bits del puerto C ******

#define	tris_c	0xBC	//configuración direcion de puerto c
#define ansel_c	0x00	//configuración modo puerto c

//******** definicion de bits del puerto D ******

#define	tris_d	0x0F	//configuración direcion de puerto d
#define ansel_d	0x00	//configuración modo puerto d

//******** definicion de bits del puerto E ******

#define	tris_e	0x00	//configuración direcion de puerto d
#define ansel_e	0x00	//configuración modo puerto d	

 #define trisd1 TRISEbits.RE0
#define trisd2 TRISEbits.RE1
#define trisd3 TRISEbits.RE2
#define trisd4 TRISCbits.RC1
#define disp1 LATEbits.LATE0
#define disp2 LATEbits.LATE1
#define disp3 LATEbits.LATE2
#define disp4 LATCbits.LATC1



#define LED1 LATDbits.LATD7
#define LED2 LATDbits.LATD6
#define LED3 LATDbits.LATD5
#define LED4 LATDbits.LATD4
 
#define MR LATBbits.LATB5
#define SCLK  LATAbits.LATA4
#define PCLK  LATAbits.LATA5
#define SDATA LATCbits.LATC0
#define BUZZER LATAbits.LATA3
 
#define tm0_h 0x8A;  // valores de recarga para el timer 0
#define tm0_l 0xB0;  // 5 ms
