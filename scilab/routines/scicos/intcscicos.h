#ifndef INTCSCICOS_SCI 
#define INTCSCICOS_SCI 

int MlistGetFieldNumber __PARAMS((int *ptr, const char *string));
int C2F(intcscicos)  _PARAMS((void));
static int inttimescicos  _PARAMS((char *fname, unsigned long fname_len));
static int intduplicate  _PARAMS((char *fname, unsigned long fname_len));
static int intdiffobjs  _PARAMS((char *fname, unsigned long fname_len));
static int intxproperty  _PARAMS((char *fname, unsigned long fname_len));
static int intphasesim  _PARAMS((char *fname, unsigned long fname_len));
static int intsetxproperty  _PARAMS((char *fname, unsigned long fname_len));
static int intcpass2  _PARAMS((char *fname, unsigned long fname_len));
static int intsetblockerror _PARAMS((char *fname, unsigned long fname_len));
static void duplicata();
static void comp_size();
extern int Max1(int* vect);
extern int *listentry(int *header, int i);
extern int Message(char* code);
extern int cpass2();

#endif 
