#ifndef MATDSR_H
#define MATDSR_H

#include "../control/control.h" 
#include "../calelm/calelm.h" 

extern int C2F(matdsr)  _PARAMS((void));
extern int C2F(intbdiagr)  _PARAMS((char *fname, unsigned long fname_len));
extern int C2F(inthess) _PARAMS((char *fname, unsigned long fname_len));
extern int C2F(intschur) _PARAMS((char *fname, unsigned long fname_len));
extern int C2F(inteig) _PARAMS((char *fname, unsigned long fname_len));
extern int C2F(intbdiagr) _PARAMS((char *fname, unsigned long fname_len));
extern int C2F(intbalanc) _PARAMS((char *fname, unsigned long fname_len));

#endif 
