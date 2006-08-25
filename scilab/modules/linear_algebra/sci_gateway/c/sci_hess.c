/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
#define SCI_DOUBLE 1
#define STRING  10
#define REAL 0
#define COMPLEX 1
#define FUNCTION 13
#define STRINGREAL 27
#define STRINGCOMPLEX 12
#define YES 1
#define NO 0
/*-----------------------------------------------------------------------------------*/
int C2F(inthess)(char *fname,unsigned long fname_len)

{
	int *header1;
	int CmplxA;int ret;

	extern int C2F(intdgehrd) __PARAMS((char *fname, unsigned long fname_len));
	extern int C2F(intzgehrd) __PARAMS((char *fname, unsigned long fname_len));

	/*   hess(A)  */
	if (GetType(1)!=1) {
		OverLoad(1);
		return 0;
	}
	header1 = (int *) GetData(1);    
	CmplxA=header1[3];   
	switch (CmplxA) {
  case REAL:   
	  ret = C2F(intdgehrd)("hess",4L);
	  return 0;
	  break;
  case COMPLEX:
	  ret = C2F(intzgehrd)("hess",4L);
	  return 0;
	  break;
  default:
	  Scierror(999,"%s: Invalid input! \r\n",fname);
	  return 0;
	  break;
	}
}
/*-----------------------------------------------------------------------------------*/
