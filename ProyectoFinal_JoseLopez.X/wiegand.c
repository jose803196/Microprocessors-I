

char ver_par(void);
#include "wiegand.h"
#include <xc.h>
// funcion que verifica la paridad retorna 1 si es valido o cero invalido
char ver_par(){
    
    char cont=0,unos_1,unos_2;
    unos_1=0;
    unsigned long int wig_val2=0;
    
    wig_val2=wig_val>>1; //roto una vez a la derecha para elimnar bit de paridad y contar los nos restants
    //******* verifico los primeros 12 unos rotando hacia la derecha
    for(cont=0;cont<=11;cont++){
        
        if(wig_val2&0x01)unos_1++; //cada vez que consigo un unolo cuento
        wig_val2=wig_val2>>1; //roto a la derache para verificar el proximo uno
        
    }
    unos_1=unos_1&0x01; //and logico para dejar solo el bit menso significativo que indica numero par o impar
    if((unos_1==0)&&(p_par==0)) return 0; //comparoconel bit de paridad recibido
    
    //repito el procedimiento con los siguientes doce bits
    unos_2=0;
     for(cont=0;cont<=11;cont++){
        
        if(wig_val2&0x01)unos_2++;
        wig_val2=wig_val2>>1;
        
    }
    unos_2=unos_2&0x01;
    if((unos_2==1)&&(p_imp==0)) return 0;
   
    return 1; //paridad verificada retorno un 1
}

//funcion wiegand se encarga de verificar si hubo un dato en la recepcion wiegand y devuelve los res bytes
char wiegand(char *b2,char *b1, char *b0){
    volatile unsigned long int s;
    if(nbits==26){ //verfico que se recibieron 26 bits
     
         
        if(!ver_par()) return 0; //si la paridad es incorrecta retorno cero
        nbits=0;        //renicio numero de bits leidos en interfaz wiegand   
        w_unos=0;       //reinicio valor de unos en wiegand
        w_ceros=0;      //reinicio valor de ceros en wiegand
        nbits0=0;       //reinicio valor de numero de ceros en wiegand
        nbits1=0;       //reinicio valor de numero de unos en wiegand
        
        p_imp=0;        //reinicio valor de paridad par
        p_par=0;        //reinicio valor de paridad impar
        
        s=(wig_val>>1)&0x000000ff;
        *b0=(char)s;
        s=(wig_val>>9)&0x000000ff;
        *b1=(char)s;
        s=(wig_val>>17)&0x000000ff;
        *b2=(char)s;
        wig_val=0;      //reinicio valor de datos en wiegand
        return 1;
    }
                else return 0; //no se han recibido 26 bits
}

void wuno(){
    
    
            timeout=20; //reinicio tiempo de timeout
            if(nbits<27){
                if(nbits==0)p_imp=1;
                if(nbits==25)p_par=1;
                
                nbits++; //incremento nuemro de bits leidos
                nbits1++;
                wig_val=wig_val<<1;
                wig_val=wig_val|0x1;
                
                w_unos=w_unos<<1;
                w_unos=w_unos|0x1;
            }
}

void wcero(){
    
     timeout=20; //reinicio tiempo de timeout
            if(nbits<27){
                 if(nbits==0)p_imp=0;
                if(nbits==25)p_par=0;
                nbits++; //incremento nuemro de bits leidos
                nbits0++;
                wig_val=wig_val<<1;
                wig_val=wig_val&0xfffffffe;
                w_ceros=w_ceros<<1;
                w_ceros=w_ceros|0x1;
            }
}

void conf_wig()
{
    ANSELA=0;
    ANSELB=0;
    ANSELC=0;
    
    
    TRISBbits.RB1=1;  //defino el bit 1 del puerto B como entrada
    TRISBbits.RB2=1;  //defino el bit 2 del puerto B como entrada
    
    INTCON2bits.INTEDG1=0;  //interrupcio por flanco de bajada
    INTCON2bits.INTEDG2=0;//intrrupcion por flanco de bajada
    INTCON3bits.INT1E=1;    //habilito interrupcion externa 1
    INTCON3bits.INT2E=1;    //habilito interrupcion externa 2
    INTCON3bits.INT1IP=1;  //alta prioridad a la interrupcion 1
    INTCON3bits.INT2IP=1;  //alta prioridad a la interrupcion 2
    
}
// funcion prs verific timeoutde recepcion wiegand
void wig_tout(){
    
     if(timeout>0)timeout--;
            else {
                nbits=0;        //renicio numero de bits leidos en interfaz wiegand   
                w_unos=0;       //reinicio valor de unos en wiegand
                w_ceros=0;      //reinicio valor de ceros en wiegand
                nbits0=0;
                nbits1=0;
                wig_val=0;
                p_imp=0;
                p_par=0;
                
                
                
            }       
}