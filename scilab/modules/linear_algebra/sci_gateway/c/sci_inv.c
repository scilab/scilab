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
int C2F(intinv)(char *fname,unsigned long fname_len)

{
	int *header1;
	int CmplxA;int ret;

	extern int C2F(intdgetri) __PARAMS((char *fname, unsigned long fname_len));
	extern int C2F(intzgetri) __PARAMS((char *fname, unsigned long fname_len));

	/*   inv(A)  */
	if (GetType(1)!=1) {
		OverLoad(1);
		return 0;
	}
	header1 = (int *) GetData(1);    
	CmplxA=header1[3];   
	switch (CmplxA) {
  case REAL:   
	  ret = C2F(intdgetri)("inv",3L);
	  return 0;
	  break;
  case COMPLEX:
	  ret = C2F(intzgetri)("inv",3L);
	  return 0;
	  break;
  default:
	  Scierror(999,"%s: Invalid input! \r\n",fname);
	  return 0;
	  break;
	}
}

/*-----------------------------------------------------------------------------------*/