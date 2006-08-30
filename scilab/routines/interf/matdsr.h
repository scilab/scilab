#ifndef MATDSR_H
#define MATDSR_H

#include "cacsd.h" 

/* REORGANISATION TEMPORAIRE */
#include "../../modules/elementaries_functions/includes/elementaries_functions.h" 

extern int C2F(matdsr)  _PARAMS((void));
extern int C2F(intbdiagr)  _PARAMS((char *fname, unsigned long fname_len));
extern int C2F(inthess) _PARAMS((char *fname, unsigned long fname_len));
extern int C2F(intschur) _PARAMS((char *fname, unsigned long fname_len));
extern int C2F(inteig) _PARAMS((char *fname, unsigned long fname_len));
extern int C2F(intbdiagr) _PARAMS((char *fname, unsigned long fname_len));
extern int C2F(intbalanc) _PARAMS((char *fname, unsigned long fname_len));

#endif 
