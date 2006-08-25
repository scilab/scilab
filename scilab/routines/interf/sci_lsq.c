/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "../stack-c.h"
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
extern int C2F(complexify)  __PARAMS((int *num));
/*-----------------------------------------------------------------------------------*/
int C2F(intlsq)(char *fname,unsigned long fname_len)
{
	int *header1;int *header2;
	int CmplxA;int Cmplxb;int ret;int I2;

	extern int C2F(intdgelsy) __PARAMS((char *fname, unsigned long fname_len));
	extern int C2F(intzgelsy) __PARAMS((char *fname, unsigned long fname_len));

	/*   lsq(A,b)  */
	if (GetType(1)!=1) {
		OverLoad(1);
		return 0;
	}
	if (GetType(2)!=1) {
		OverLoad(2);
		return 0;
	}
	header1 = (int *) GetData(1);    header2 = (int *) GetData(2);
	CmplxA=header1[3];   Cmplxb=header2[3];
	switch (CmplxA) {
  case REAL:   
	  switch (Cmplxb) {
  case REAL :
	  /* A real, b real */
	  ret = C2F(intdgelsy)("lsq",3L);
	  break;
  case COMPLEX :
	  /* A real, b complex */
	  C2F(complexify)((I2=1,&I2));
	  ret = C2F(intzgelsy)("lsq",3L);
	  break;
  default:
	  break;
	  }
	  return 0;
  case COMPLEX :
	  switch (Cmplxb) {
  case REAL :
	  /* A complex, b real */
	  C2F(complexify)((I2=2,&I2));
	  ret = C2F(intzgelsy)("lsq",3L);
	  break;
  case COMPLEX :
	  /* A complex, b complex */
	  ret = C2F(intzgelsy)("lsq",3L);
	  break;
  default:
	  Scierror(999,"%s: Invalid input! \r\n",fname);
	  break;
	  }
	  return 0;
	  break;
  default :
	  Scierror(999,"%s: Invalid input! \r\n",fname);
	  return 0;
	  break;
	}
}
/*-----------------------------------------------------------------------------------*/