/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "stack-c.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
/*-----------------------------------------------------------------------------------*/
extern int C2F(intdgetrf) __PARAMS((char *fname, unsigned long fname_len));
extern int C2F(intzgetrf) __PARAMS((char *fname, unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
int C2F(intlu)(char *fname,unsigned long fname_len)
{
	int *header1;
	int CmplxA;int ret;

	/*   lu(A)  */
	if (GetType(1)!=sci_matrix) {
		OverLoad(1);
		return 0;
	}
	header1 = (int *) GetData(1);
	CmplxA=header1[3];
	switch (CmplxA) {
  case REAL:
	  ret = C2F(intdgetrf)("lu",2L);
	  return 0;
	  break;
  case COMPLEX:
	  ret = C2F(intzgetrf)("lu",2L);
	  return 0;
	  break;
  default:
	  Scierror(999,"%s: Invalid input! \n",fname);
	  return 0;
	}
}
/*-----------------------------------------------------------------------------------*/
