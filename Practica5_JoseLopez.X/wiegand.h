
#include <xc.h> // include processor files - each processor file is guarded.  



char wiegand(char *b1, char *b2,char *b3);
void wuno(void);
void wcero(void);
void conf_wig(void);
void wig_tout(void);

char timeout;
char nbits;
char nbits0;
char nbits1;
unsigned int w_unos;
unsigned int w_ceros;
unsigned long int wig_val;
char p_imp;
char p_par;

