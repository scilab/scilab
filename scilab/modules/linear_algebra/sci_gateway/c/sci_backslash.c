/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "machine.h"
#include "stack-c.h"
#include "gw_linear_algebra.h"
/*-----------------------------------------------------------------------------------*/
extern int C2F(complexify)  __PARAMS((int *num));
extern int C2F(intdgesv3) __PARAMS((char *fname, unsigned long fname_len));
extern int C2F(intzgesv3) __PARAMS((char *fname, unsigned long fname_len));

/*-----------------------------------------------------------------------------------*/
int C2F(intbackslash)(char *fname,unsigned long fname_len)
{
	int *header1;int *header2;
	int CmplxA;int CmplxB;int ret;int X;

	/*   backslash(A,B)  */
	header1 = (int *) GetData(1);    
	header2 = (int *) GetData(2);
	CmplxA=header1[3];   CmplxB=header2[3];
	if ((header1[1]!=header2[1])&(header2[1]*header2[2]==1)) {
		C2F(com).fun=0;
		C2F(com).fin=-C2F(com).fin;
		return 0;
	}
	switch (CmplxA) {
  case REAL:   
	  switch (CmplxB) {
  case REAL :
	  /* A real, B real */
	  ret = C2F(intdgesv3)("lsq",3L);
	  break;
  case COMPLEX :
	  /* A real, B complex : complexify A */
	  C2F(complexify)((X=1,&X));
	  ret = C2F(intzgesv3)("lsq",3L);
	  break;
  default:
	  Scierror(999,"%s: Invalid input! \n",fname);
	  break;
	  }
	  return 0;
  case COMPLEX :
	  switch (CmplxB) {
  case REAL :
	  /* A complex, B real : complexify B */
	  C2F(complexify)((X=2,&X));
	  ret = C2F(intzgesv3)("lsq",3L);
	  break;
  case COMPLEX :
	  /* A complex, B complex */
	  ret = C2F(intzgesv3)("lsq",3L);
	  break;
  default:
	  Scierror(999,"%s: Invalid input! \n",fname);
	  break;
	  }
	  return 0;
  default :
	  Scierror(999,"%s: Invalid input! \n",fname);
	  return 0;
	  break;
	}
}
/*-----------------------------------------------------------------------------------*/
