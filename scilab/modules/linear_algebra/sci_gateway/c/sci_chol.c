/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "stack-c.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
/*-----------------------------------------------------------------------------------*/
extern int C2F(intdpotrf) __PARAMS((char *fname, unsigned long fname_len));
extern int C2F(intzpotrf) __PARAMS((char *fname, unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
int C2F(intchol)(char *fname,unsigned long fname_len)
{
	int *header1;
	int CmplxA;int ret;

	/*   chol(A)  */
	if (GetType(1)!=1) {
		OverLoad(1);
		return 0;
	}
	header1 = (int *) GetData(1);
	CmplxA=header1[3];
	switch (CmplxA) {
  case REAL:
	  ret = C2F(intdpotrf)("chol",4L);
	  return 0;
	  break;
  case COMPLEX:
	  ret = C2F(intzpotrf)("chol",4L);
	  return 0;
	  break;
  default:
	  Scierror(999,"%s: Invalid input! \n",fname);
	  return 0;
	  break;
	}
}
/*-----------------------------------------------------------------------------------*/
