/*--------------------------------------------------------------------------*/ 
/* INRIA */
/*--------------------------------------------------------------------------*/ 
#include <string.h>
#include "machine.h"
#include "stack-c.h"
#include "sci_rankqr.h"
/*--------------------------------------------------------------------------*/ 
extern int C2F(intmb03od) __PARAMS((char *fname, unsigned long fname_len));
extern int C2F(intzb03od) __PARAMS((char *fname, unsigned long fname_len));
/*--------------------------------------------------------------------------*/ 
int intrankqr(char* fname)
{
	int *header1;
	int Cmplx;int ret;

	header1 = (int *) GetData(1);
	Cmplx=header1[3];
	if (Cmplx==0) {
		ret = C2F(intmb03od)("rankqr",6L);
		return 0; } 
	else
	{
		ret = C2F(intzb03od)("rankqr",6L);
		return 0; } 
}
/*--------------------------------------------------------------------------*/ 
