#include "../stack-c.h"
#include <string.h>
#include <stdio.h>

#define DOUBLE 1
#define STRING  10
#define REAL 0
#define COMPLEX 1
#define FUNCTION 13
#define STRINGREAL 27
#define STRINGCOMPLEX 12

/* #define Abs(x) ( (x) > 0) ? (x) : -(x) */

extern int C2F(intlapack)  __PARAMS((void));


int intqr(fname)
     char *fname;
{
  int *header1;int *header2;
  int Cmplx;int ret; double *snd; double tol;

  header1 = (int *) GetData(1);
  Cmplx=header1[3];

  if (header1[0] == 10) Cmplx=10;

  if (Lhs==4) {   /* obsolete : [Q,R,rk,E]=qr(A) or = qr(A,tol)   */
    if (Rhs==2) {
      snd = (double *) GetData(2);
      tol = snd[2];
    }
    else {
      tol = -1;Rhs=1;
    }
    switch (Cmplx) {
    case REAL :
       ret = C2F(doldqr)(&tol,"qr",2L);
       break;
    case COMPLEX :
      ret = C2F(zoldqr)(&tol,"qr",2L); 
      break;
    default :
      Scierror(999,"%s: Invalid input! \r\n",fname);
      return;
    }
    return;
  }
  switch (Rhs) {
  case 1:   /*   qr(A)   */
    switch (Cmplx) {
    case REAL :
      ret = C2F(intdgeqpf3)("qr",2L);
      break;
    case COMPLEX :
      ret = C2F(intzgeqpf3)("qr",2L);
      break;
    default :
      Scierror(999,"%s: Invalid input! \r\n",fname);
      break;
    }
    break;
  case 2 :   /*   qr(A, something)   */
    header2 = (int *) GetData(2);
    switch (header2[0]) {
    case STRING  :
      /* Economy size:  ...=qr(A,"e")  */
      switch (Cmplx) {
      case REAL :
	ret = C2F(intdgeqpf4)("qr",2L);
	break;
      case COMPLEX :
	ret = C2F(intzgeqpf4)("qr",2L);  
	break;
      default :
	Scierror(999,"%s: Invalid input! \r\n",fname);
	break;
      }
    default:
      Scierror(999,"%s: Invalid input! \r\n",fname);
      break;
    }
    return;
  default :
    Scierror(999,"%s: Invalid call! \r\n",fname);
    break;
  }
  return 0;
}


int intsvd(fname)
     char *fname;
{
  int *header1;int *header2;
  int Cmplx;int ret;

  header1 = (int *) GetData(1);
  Cmplx=header1[3];

  switch (Rhs) {
  case 1:   /* svd(A)   */
    if (Cmplx==0) {
      ret = C2F(intdgesvd1)("svd",3L);
      return; }
    if (Cmplx==1) {
      ret = C2F(intzgesvd1)("svd",3L);
      return; } 
    break;
  case 2 :   /* svd(A, something)   */
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
    Scierror(999,"%s: Invalid call! \r\n",fname);
    break;
  }
}


int intlsq(fname)
  char *fname;
{
  int *header1;int *header2;
  int CmplxA;int Cmplxb;int ret;int I2;

  /*   lsq(A,b)  */
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
    return;
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
    return;
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
  case REAL:   
    ret = C2F(intdgeev)("spec",4L);
    return;
    break;
  case COMPLEX:
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
  case REAL:   
    ret = C2F(intdgetri)("inv",3L);
    return;
    break;
  case COMPLEX:
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
  case REAL:   
    ret = C2F(intdgecon)("rcond",5L);
    return;
    break;
  case COMPLEX:
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
  case REAL:   
    ret = C2F(intdpotrf)("chol",4L);
    return;
    break;
  case COMPLEX:
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

  /*   lu(A)  */
  header1 = (int *) GetData(1);    
  CmplxA=header1[3];   
  switch (CmplxA) {
  case REAL:   
    ret = C2F(intdgetrf)("lu",2L);
    return;
    break;
  case COMPLEX:
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
  int CmplxA;int CmplxB;int ret;int X;

  /*   X = slash(A,B) <=> X = B / A */
  header1 = (int *) GetData(1);    header2 = (int *) GetData(2);
  CmplxA=header1[3];   CmplxB=header2[3];
  switch (CmplxA) {
  case REAL:   
    switch (CmplxB) {
    case REAL :
      /* A real, Breal */
      ret = C2F(intdgesv4)("slash",5L);
      return;
      break;
    case COMPLEX :
      /* A real, B complex : complexify A */
      C2F(complexify)((X=1,&X));
      ret = C2F(intzgesv4)("slash",5L);
      return;
      break;
    default:
      Scierror(999,"%s: Invalid input! \r\n",fname);
      return;
      break;
    }
  case COMPLEX :
    switch (CmplxB) {
    case REAL :
      /* A complex, B real : complexify B */
      C2F(complexify)((X=2,&X));
      ret = C2F(intzgesv4)("slash",5L);
      return;
      break;
    case COMPLEX :
      /* A complex, B complex */
      ret = C2F(intzgesv4)("slash",5L);
      return;
      break;
    default:
      Scierror(999,"%s: Invalid input! \r\n",fname);
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
  int CmplxA;int CmplxB;int ret;int X;

  /*   backslash(A,B)  */
  header1 = (int *) GetData(1);    header2 = (int *) GetData(2);
  CmplxA=header1[3];   CmplxB=header2[3];
  switch (CmplxA) {
  case REAL:   
    switch (CmplxB) {
    case REAL :
      /* A real, B real */
      ret = C2F(intdgesv3)("lsq",3L);
      return;
      break;
    case COMPLEX :
      /* A real, B complex : complexify A */
      C2F(complexify)((X=1,&X));
      ret = C2F(intzgesv3)("lsq",3L);
      return;
      break;
    default:
      Scierror(999,"%s: Invalid input! \r\n",fname);
      return;
      break;
    }
  case COMPLEX :
    switch (CmplxB) {
    case REAL :
      /* A complex, B real : complexify B */
      C2F(complexify)((X=2,&X));
      ret = C2F(intzgesv3)("lsq",3L);
      return;
      break;
    case COMPLEX :
      /* A complex, B complex */
      ret = C2F(intzgesv3)("lsq",3L);
      return;
      break;
    default:
      Scierror(999,"%s: Invalid input! \r\n",fname);
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

int intschur(fname)
     char *fname;

{
  int *header1;int *header2; int Cmplx;int ret; int something;
  int which = 1; int i; int longueur;
  header1 = (int *) GetData(1);   Cmplx=header1[3];
  switch (Rhs) {
  case 1:   /* schur(A)   */
    switch (Cmplx) {
    case REAL :
      ret = C2F(intdgees0)("schur",5L);
      break;
    case COMPLEX :
      ret = C2F(intzgees0)("schur",5L);
      break;
    default :
      Scierror(999,"%s: Invalid input! \r\n",fname);
      break;
    }
    return;
  case 2 :   /* schur(A, something)   */
    header2 = (int *) GetData(2); something=header2[0];
    switch (something) {
    case FUNCTION :
      switch (Cmplx) {
      case REAL :
	ret = C2F(intoschur)("schur",5L);
	break;
      case COMPLEX :
	ret = C2F(intzschur)("schur",5L);
	break;
      default :
	Scierror(999,"%s: Invalid input! \r\n",fname);
	break;
      }
      break;
    case STRING  :
      longueur=header2[5]-header2[4];
      which = schtst(longueur, header2);
      switch (which) {
      case STRINGREAL:
	switch (Cmplx) {
	case REAL:
	  ret = C2F(intdgees1)("schur",5L);
	  break;
	case COMPLEX:
	  Scierror(999,"%s: Invalid call! \r\n",fname);
	  break;
	}
	return;
      case STRINGCOMPLEX:
	switch (Cmplx) {
	case REAL:
	  ret = C2F(intdgees1)("schur",5L);
	  break;
	case COMPLEX:
	  ret = C2F(intzgees1)("schur",5L);
	  break;
	}
	return;
      default:
	/*   String is an external function  */
	switch (Cmplx) {
	case REAL:
	  ret = C2F(intfschur)("schur",5L);
	  break;
	case COMPLEX:
	  ret = C2F(intzfschur)("schur",5L);
	  break;
	}
	return;
      }
    default :
      Scierror(999,"%s: Invalid call! \r\n",fname);
      break;
    }
  }
  return 0;
}

int schtst (longueur, header)
     int longueur;
     int *header;
{
  if( (longueur==1) && ( Abs(header[6])==27 ) ) return STRINGREAL;  /* "r" */
  if( (longueur==4) && ( Abs(header[6])==27 ) && (Abs(header[7])==14 ) && (Abs(header[6])==10 ) && ( Abs(header[6])==21 ) ) return STRINGREAL;  /* "real"  */
  if( (longueur==4) && ( Abs(header[6])==12 ) && (Abs(header[7])==24 ) && (Abs(header[6])==22 ) && ( Abs(header[6])==25 ) ) return STRINGCOMPLEX;  /* "comp" */
  if( (longueur==7) && ( Abs(header[6])==12 ) && (Abs(header[7])==24 ) && (Abs(header[8])==22 ) && ( Abs(header[9])==25 ) && ( Abs(header[10])==21) && ( Abs(header[11])==14) && ( Abs(header[12])==33) ) return STRINGCOMPLEX;  /* "complex" */
return 0;
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
  {intschur,"lap_schur"},
};

int C2F(intlapack)()
{  
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
  C2F(putlhsvar)();
  return 0;
}
