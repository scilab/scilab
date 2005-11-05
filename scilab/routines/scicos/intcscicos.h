#ifndef INTCSCICOS_SCI 
#define INTCSCICOS_SCI 

int MlistGetFieldNumber __PARAMS((int *ptr, const char *string));
int C2F(intcscicos)  _PARAMS((void));
int inttimescicos  _PARAMS((char *fname, unsigned long fname_len));
int intduplicate  _PARAMS((char *fname, unsigned long fname_len));
int intdiffobjs  _PARAMS((char *fname, unsigned long fname_len));
int intxproperty  _PARAMS((char *fname, unsigned long fname_len));
int intphasesim  _PARAMS((char *fname, unsigned long fname_len));
int intsetxproperty  _PARAMS((char *fname, unsigned long fname_len));
int intcpass2  _PARAMS((char *fname, unsigned long fname_len));
int intsetblockerror _PARAMS((char *fname, unsigned long fname_len));
int inttree2 _PARAMS((char *fname, unsigned long fname_len));
int inttree3 _PARAMS((char *fname, unsigned long fname_len));
int inttree4 _PARAMS((char *fname, unsigned long fname_len));
static void duplicata();
static void comp_size();
extern int Max1(int* vect);
extern int *listentry(int *header, int i);
extern int Message(char* code);
extern int cpass2();


#endif 
