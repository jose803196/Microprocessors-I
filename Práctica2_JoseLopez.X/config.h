

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
