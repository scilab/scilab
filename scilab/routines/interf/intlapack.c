#include "../stack-c.h"
#include <string.h>
#include <stdio.h>

#define DOUBLE 1
#define STRING  10

extern int C2F(intlapack)  __PARAMS((void));


int intqr()
{
  int *header1;int *header2;
  int Cmplx;int ret;

  header1 = (int *) GetData(1);
  Cmplx=header1[3];

  switch (Rhs) {
  case 1:   /* ...=qr(A)   */
    if (Cmplx==0) {
      ret = C2F(intdgeqpf3)("qr",2L);
      return; }
    if (Cmplx==1) {
      ret = C2F(intzgeqpf3)("qr",2L);
      return; } 
    break;
  case 2 :   /* ...=qr(A, something)   */
    header2 = (int *) GetData(2);
    switch (header2[0]) {
    case DOUBLE :
      /*  old qr, tolerance is passed: [Q,R,rk,E]=qr(A,tol)  */
      /*   ret = C2F(intqrold)("qr",2L);  */
      return;
      break;
    case STRING  :
      /* Economy size:  ...=qr(A,"e")  */
      if (Cmplx==0) {
	ret = C2F(intdgeqpf4)("qr",2L);  
	return;}
      if (Cmplx==1) {
	ret = C2F(intzgeqpf4)("qr",2L);  
	return;} 
      break;
    }
    break;
  default :   /*  rhs > 2 */
    break;
  }
}

int intsvd()
{
  int *header1;int *header2;
  int Cmplx;int ret;

  header1 = (int *) GetData(1);
  Cmplx=header1[3];

  switch (Rhs) {
  case 1:   /* ...=svd(A)   */
    if (Cmplx==0) {
      ret = C2F(intdgesvd1)("svd",3L);
      return; }
    if (Cmplx==1) {
      ret = C2F(intzgesvd1)("svd",3L);
      return; } 
    break;
  case 2 :   /* ...=svd(A, something)   */
    header2 = (int *) GetData(2);
    switch (header2[0]) {
    case DOUBLE :
      /*  old svd, tolerance is passed: [U,S,V,rk]=svd(A,tol)  */
      /*   ret = C2F(intsvdold)("svd",2L);  */
      return;
      break;
    case STRING  :
      /* Economy size:  [U,S,V]=svd(A,"e")  */
      if (Cmplx==0) {
	ret = C2F(intdgesvd2)("svd",3L);  
	return;}
      if (Cmplx==1) {
	ret = C2F(intzgesvd2)("svd",3L);  
	return;} 
      break;
    }
    break;
  default :   /*  rhs > 2 */
    break;
  }
}


int intlsq(fname)
  char *fname;
{
  int *header1;int *header2;
  int CmplxA;int Cmplxb;int ret;

  /*   lsq(A,b)  */
  header1 = (int *) GetData(1);    header2 = (int *) GetData(2);
  CmplxA=header1[3];   Cmplxb=header2[3];
  switch (CmplxA) {
  case 0:   
    switch (Cmplxb) {
    case 0 :
      /* A real, breal */
      ret = C2F(intdgelsy)("lsq",3L);
      return;
      break;
    case 1 :
      /* A real, b complex : to be done */
      Scierror(999,"%s: Invalid input! \r\n",fname);
      return;
      break;
    default:
      return;
      break;
    }
  case 1 :
    switch (Cmplxb) {
    case 0 :
      /* A complex, b real : to be done */
      Scierror(999,"%s: Invalid input! \r\n",fname);
      return;
      break;
    case 1 :
      /* A complex, b complex */
      ret = C2F(intzgelsy)("lsq",3L);
      return;
      break;
    default:
      return;
      break;
    }
    break;
  default :
    Scierror(999,"%s: Invalid input! \r\n",fname);
    return;
    break;
  }
}




int inteig(fname)
  char *fname;
{
  int *header1;
  int CmplxA;int ret;

  /*   spec(A)  */
  header1 = (int *) GetData(1);    
  CmplxA=header1[3];   
  switch (CmplxA) {
  case 0:   
    ret = C2F(intdgeev)("spec",4L);
    return;
    break;
  case 1:
    ret = C2F(intzgeev)("spec",4L);
    return;
    break;
  default:
    Scierror(999,"%s: Invalid input! \r\n",fname);
    return 0;
    break;
  }
}

int intinv(fname)
  char *fname;
{
  int *header1;
  int CmplxA;int ret;

  /*   inv(A)  */
  header1 = (int *) GetData(1);    
  CmplxA=header1[3];   
  switch (CmplxA) {
  case 0:   
    ret = C2F(intdgetri)("inv",3L);
    return;
    break;
  case 1:
    ret = C2F(intzgetri)("inv",3L);
    return;
    break;
  default:
    Scierror(999,"%s: Invalid input! \r\n",fname);
    return 0;
    break;
  }
}

int intrcond(fname)
  char *fname;
{
  int *header1;
  int CmplxA;int ret;

  /*   rcond(A)  */
  header1 = (int *) GetData(1);    
  CmplxA=header1[3];   
  switch (CmplxA) {
  case 0:   
    ret = C2F(intdgecon)("rcond",5L);
    return;
    break;
  case 1:
    ret = C2F(intzgecon)("rcond",5L);
    return;
    break;
  default:
    Scierror(999,"%s: Invalid input! \r\n",fname);
    return 0;
    break;
  }
}

int intchol(fname)
  char *fname;
{
  int *header1;
  int CmplxA;int ret;

  /*   chol(A)  */
  header1 = (int *) GetData(1);    
  CmplxA=header1[3];   
  switch (CmplxA) {
  case 0:   
    ret = C2F(intdpotrf)("chol",4L);
    return;
    break;
  case 1:
    ret = C2F(intzpotrf)("chol",4L);
    return;
    break;
  default:
    Scierror(999,"%s: Invalid input! \r\n",fname);
    return 0;
    break;
  }
}

int intlu(fname)
  char *fname;
{
  int *header1;
  int CmplxA;int ret;

  /*   chol(A)  */
  header1 = (int *) GetData(1);    
  CmplxA=header1[3];   
  switch (CmplxA) {
  case 0:   
    ret = C2F(intdgetrf)("lu",2L);
    return;
    break;
  case 1:
    ret = C2F(intzgetrf)("lu",2L);
    return;
    break;
  default:
    Scierror(999,"%s: Invalid input! \r\n",fname);
    return 0;
    break;
  }
}

int intslash(fname)
  char *fname;
{
  int *header1;int *header2;
  int CmplxA;int CmplxB;int ret;

  /*   X = slash(A,B) <=> X = B / A */
  header1 = (int *) GetData(1);    header2 = (int *) GetData(2);
  CmplxA=header1[3];   CmplxB=header2[3];
  switch (CmplxA) {
  case 0:   
    switch (CmplxB) {
    case 0 :
      /* A real, Breal */
      ret = C2F(intdgesv4)("slash",5L);
      return;
      break;
    case 1 :
      /* A real, B complex : to be done */
      Scierror(999,"%s: Invalid input! \r\n",fname);
      return;
      break;
    default:
      return;
      break;
    }
  case 1 :
    switch (CmplxB) {
    case 0 :
      /* A complex, B real : to be done */
      Scierror(999,"%s: Invalid input! \r\n",fname);
      return;
      break;
    case 1 :
      /* A complex, B complex */
      ret = C2F(intzgesv4)("slash",5L);
      return;
      break;
    default:
      return;
      break;
    }
    break;
  default :
    Scierror(999,"%s: Invalid input! \r\n",fname);
    return;
    break;
  }
}


int intbackslash(fname)
  char *fname;
{
  int *header1;int *header2;
  int CmplxA;int CmplxB;int ret;

  /*   lsq(A,b)  */
  header1 = (int *) GetData(1);    header2 = (int *) GetData(2);
  CmplxA=header1[3];   CmplxB=header2[3];
  switch (CmplxA) {
  case 0:   
    switch (CmplxB) {
    case 0 :
      /* A real, B real */
      ret = C2F(intdgesv3)("lsq",3L);
      return;
      break;
    case 1 :
      /* A real, B complex : to be done */
      Scierror(999,"%s: Invalid input! \r\n",fname);
      return;
      break;
    default:
      return;
      break;
    }
  case 1 :
    switch (CmplxB) {
    case 0 :
      /* A complex, B real : to be done */
      Scierror(999,"%s: Invalid input! \r\n",fname);
      return;
      break;
    case 1 :
      /* A complex, B complex */
      ret = C2F(intzgesv3)("lsq",3L);
      return;
      break;
    default:
      return;
      break;
    }
    break;
  default :
    Scierror(999,"%s: Invalid input! \r\n",fname);
    return;
    break;
  }
}


typedef int (*des_interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct {
  des_interf f;    /** function **/
  char *name;      /** its name **/
} LapackTable;

 
static LapackTable Tab[]={
  {intqr,"lap_qr"},
  {intsvd,"lap_svd"},
  {intlsq,"lsq"},
  {inteig,"lap_spec"},
  {intinv,"lap_inv"},
  {intrcond,"lap_rcond"},
  {intchol,"lap_chol"},
  {intlu,"lap_lu"},
  {intslash,"lap_slash"},
  {intbackslash,"lap_backslash"},
};

int C2F(intlapack)()
{  
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
  C2F(putlhsvar)();
  return 0;
}
