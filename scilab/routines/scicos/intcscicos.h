#ifndef INTCSCICOS_SCI 
#define INTCSCICOS_SCI 

extern int C2F(intcscicos)  _PARAMS((void));
extern int inttimescicos  _PARAMS((char *fname, unsigned long fname_len));
extern int intduplicate  _PARAMS((char *fname, unsigned long fname_len));
extern int intdiffobjs  _PARAMS((char *fname, unsigned long fname_len));
extern int intxproperty  _PARAMS((char *fname, unsigned long fname_len));
extern int intphasesim  _PARAMS((char *fname, unsigned long fname_len));
extern int intsetxproperty  _PARAMS((char *fname, unsigned long fname_len));
extern int intcpass2  _PARAMS((char *fname, unsigned long fname_len));
extern int intsetblockerror _PARAMS((char *fname, unsigned long fname_len));
void duplicata();
void comp_size();
extern int Max1(int* vect);
extern int *listentry(int *header, int i);
extern int Message(char* code);
extern int cpass2();

#endif 
