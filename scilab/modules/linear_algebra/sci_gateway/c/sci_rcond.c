/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "stack-c.h"
#include "gw_linear_algebra.h"
/*-----------------------------------------------------------------------------------*/
extern int C2F(intdgecon) __PARAMS((char *fname, unsigned long fname_len));
extern int C2F(intzgecon) __PARAMS((char *fname, unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
int C2F(intrcond)(char *fname,unsigned long fname_len)
{
	int *header1;
	int CmplxA;int ret;


	/*   rcond(A)  */
	if (GetType(1)!=1) {
		OverLoad(1);
		return 0;
	}
	header1 = (int *) GetData(1);    
	CmplxA=header1[3];   
	switch (CmplxA) {
  case REAL:   
	  ret = C2F(intdgecon)("rcond",5L);
	  return 0;
	  break;
  case COMPLEX:
	  ret = C2F(intzgecon)("rcond",5L);
	  return 0;
	  break;
  default:
	  Scierror(999,"%s: Invalid input! \n",fname);
	  return 0;
	  break;
	}
}
/*-----------------------------------------------------------------------------------*/
