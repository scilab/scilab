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
#define YES 1
#define NO 0

/* #define Abs(x) ( (x) > 0) ? (x) : -(x) */

extern int C2F(intlapack)  __PARAMS((void));

extern int C2F(complexify)  __PARAMS((int *num));
extern int C2F(issymmetric)  __PARAMS((int *num));

int C2F(intqr)(fname)
     char *fname;
{
  int *header1;int *header2;
  int Cmplx;int ret; double *snd; double tol;

  extern int C2F(intdgeqpf3) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzgeqpf3) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intdgeqpf4) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzgeqpf4) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(doldqr) __PARAMS((double *tol,char *fname, unsigned long fname_len));
  extern int C2F(zoldqr) __PARAMS((double *tol,char *fname, unsigned long fname_len));

  if (GetType(1)!=1) {
    OverLoad(1);
    return 0;
  }
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
      return 0;
    }
    return 0;
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
      break;
    default:
      Scierror(999,"%s: Invalid input! \r\n",fname);
      break;
    }
    return 0;
  default :
    Scierror(999,"%s: Invalid call! \r\n",fname);
    break;
  }
  return 0;
}


int C2F(intsvd)(fname)
     char *fname;
{
  int *header1;int *header2;
  int Cmplx;int ret;double tol;

  extern int C2F(intdgesvd1) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzgesvd1) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intdgesvd2) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzgesvd2) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intdoldsvd) __PARAMS((double *tol, char *fname, unsigned long fname_len));
  extern int C2F(intzoldsvd) __PARAMS((double *tol, char *fname, unsigned long fname_len));
  if (GetType(1)!=1) {
    OverLoad(1);
    return 0;
  }
  header1 = (int *) GetData(1);
  Cmplx=header1[3];

  switch (Rhs) {
  case 1:   /* s=svd(A)   or   [U,s,V]=svd(A)   */
    switch (Lhs) {
    case 1: case 2: case 3:
      if (Cmplx==0) {
	ret = C2F(intdgesvd1)("svd",3L);
	return 0; }
      if (Cmplx==1) {
	ret = C2F(intzgesvd1)("svd",3L);
	return 0; } 
      break;
    case 4:
      if (Cmplx==0) {
	ret = C2F(intdoldsvd)((tol=0,&tol),"svd",3L);  
	return 0;}
      if (Cmplx==1) {
	ret = C2F(intzoldsvd)((tol=0,&tol),"svd",3L);  
	return 0;} 
      break;
    }
  case 2 :   /* svd(A, something)   */
    header2 = (int *) GetData(2);
    switch (header2[0]) {
    case DOUBLE :
      if (Lhs == 4) {
	/*  old svd, tolerance is passed: [U,S,V,rk]=svd(A,tol)  */
	/*   ret = C2F(intsvdold)("svd",2L);  */
	if (Cmplx==0) {
	  tol = ((double *) header2)[2];
	  ret = C2F(intdoldsvd)(&tol,"svd",3L);  
	  return 0;}
	if (Cmplx==1) {
	  tol = ((double *) header2)[2];
	  ret = C2F(intzoldsvd)(&tol,"svd",3L);  
	  return 0;} 
      }
      else {
	/* old Economy size:  [U,S,V]=svd(A,0)  */
	if (Cmplx==0) {
	  ret = C2F(intdgesvd2)("svd",3L);  
	  return 0;}
	if (Cmplx==1) {
	  ret = C2F(intzgesvd2)("svd",3L);  
	  return 0;} 
      }
      break;
    case STRING  :
      /* Economy size:  [U,S,V]=svd(A,"e")  */
      if (Cmplx==0) {
	ret = C2F(intdgesvd2)("svd",3L);  
	return 0;}
      if (Cmplx==1) {
	ret = C2F(intzgesvd2)("svd",3L);  
	return 0;} 
      break;
    }
    break;
  default :   /*  rhs > 2 */
    Scierror(999,"%s: Invalid call! \r\n",fname);
    break;
  }
  return 0;
}


int C2F(intlsq)(fname)
  char *fname;
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

int C2F(inteig)(fname)
  char *fname;
{
  int *header1, *header2;
  int CmplxA, CmplxB;
  int ret;int Symmetric;int X;

  extern int C2F(intdgeev) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intdsyev) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzgeev) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzheev) __PARAMS((char *fname, unsigned long fname_len));

  extern int C2F(intdggev) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzggev) __PARAMS((char *fname, unsigned long fname_len));

  switch (Rhs) {
  case 1:   /* spec(A)   */
    if (GetType(1)!=1) {
      OverLoad(1);
      return 0;
    }
    header1 = (int *) GetData(1);    
    CmplxA=header1[3];
    Symmetric = C2F(issymmetric)((X=1,&X));
    switch (CmplxA) {
    case REAL:
      switch (Symmetric) {
      case NO :
	ret = C2F(intdgeev)("spec",4L);
	break;
      case YES :
	ret = C2F(intdsyev)("spec",4L);
	break;
      }
      break;
    case COMPLEX:
      switch (Symmetric) {
      case NO :
	ret = C2F(intzgeev)("spec",4L);
	break;
      case YES: 
	ret = C2F(intzheev)("spec",4L);
	break;
      }
      break;
    default:
      Scierror(999,"%s: Invalid input! \r\n",fname);
      break;
    } /* end switch  (CmplxA) */
    break; /* end case 1 */
  case 2: /* gspec(A,B) */
    if (GetType(1)!=1) {
      OverLoad(1);
      return 0;
    }
    if (GetType(2)!=1) {
      OverLoad(2);
      return 0;
    }
    header1 = (int *) GetData(1);    
    header2 = (int *) GetData(2);
    CmplxA=header1[3];   
    CmplxB=header2[3];
    switch (CmplxA) {
    case REAL:   
      switch (CmplxB) {
      case REAL :
	/* A real, Breal */
	ret = C2F(intdggev)("gspec",5L);
	break;
      case COMPLEX :
	/* A real, B complex : complexify A */
	C2F(complexify)((X=1,&X));
	ret = C2F(intzggev)("gspec",5L);
	break;
      default:
	Scierror(999,"%s: Invalid input! \r\n",fname);
	break;
      }
      break;
    case COMPLEX :
      switch (CmplxB) {
      case REAL :
	/* A complex, B real : complexify B */
	C2F(complexify)((X=2,&X));
	ret = C2F(intzggev)("gspec",5L);
	break;
      case COMPLEX :
	/* A complex, B complex */
	ret = C2F(intzggev)("gspec",5L);
	break;
      default:
	Scierror(999,"%s: Invalid input! \r\n",fname);
	break;
      }
      break;
    default :
      Scierror(999,"%s: Invalid input! \r\n",fname);
      break;
    } /*end  switch (CmplxA) */
    break;/* end case 2 */
  }/* end switch (Rhs) */
  return 0;
} 

int C2F(intinv)(fname)
  char *fname;
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

int C2F(intrcond)(fname)
  char *fname;
{
  int *header1;
  int CmplxA;int ret;


  extern int C2F(intdgecon) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzgecon) __PARAMS((char *fname, unsigned long fname_len));

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
    Scierror(999,"%s: Invalid input! \r\n",fname);
    return 0;
    break;
  }
}

int C2F(intchol)(fname)
  char *fname;
{
  int *header1;
  int CmplxA;int ret;

  extern int C2F(intdpotrf) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzpotrf) __PARAMS((char *fname, unsigned long fname_len));

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
    Scierror(999,"%s: Invalid input! \r\n",fname);
    return 0;
    break;
  }
}

int C2F(inthess)(fname)
  char *fname;
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

int C2F(intlu)(fname)
  char *fname;
{
  int *header1;
  int CmplxA;int ret;

  extern int C2F(intdgetrf) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzgetrf) __PARAMS((char *fname, unsigned long fname_len));

  /*   lu(A)  */
  if (GetType(1)!=1) {
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
    Scierror(999,"%s: Invalid input! \r\n",fname);
    return 0;
  }
  return 0;
}
int C2F(intdet)(fname)
  char *fname;
{
  int *header1;
  int CmplxA;int ret;

  extern int C2F(intddet) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzdet) __PARAMS((char *fname, unsigned long fname_len));


  /*   det(A)  */
  if (GetType(1)!=1) {
    OverLoad(1);
    return 0;
  }
  header1 = (int *) GetData(1);    
  CmplxA=header1[3];   
  switch (CmplxA) {
  case REAL:   
    ret = C2F(intddet)("det",3L);
    return 0;
    break;
  case COMPLEX:
    ret = C2F(intzdet)("det",3L);
    return 0;
    break;
  default:
    Scierror(999,"%s: Invalid input! \r\n",fname);
    return 0;
  }
  return 0;
}

int C2F(intslash)(fname)
  char *fname;
{
  int *header1;int *header2;
  int CmplxA;int CmplxB;int ret;int X;

  extern int C2F(intdgesv4) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzgesv4) __PARAMS((char *fname, unsigned long fname_len));

  /*   X = slash(A,B) <=> X = A / B */
  header1 = (int *) GetData(1);    header2 = (int *) GetData(2);
  CmplxA=header1[3];   CmplxB=header2[3];
  if ((header1[2]!=header2[2])&(header1[1]*header1[2]==1)) {
    C2F(com).fun=0;
    C2F(com).fin=-C2F(com).fin;
    return 0;
  }
  switch (CmplxA) {
  case REAL:   
    switch (CmplxB) {
    case REAL :
      /* A real, Breal */
      ret = C2F(intdgesv4)("slash",5L);
      break;
    case COMPLEX :
      /* A real, B complex : complexify A */
      C2F(complexify)((X=1,&X));
      ret = C2F(intzgesv4)("slash",5L);
      break;
    default:
      Scierror(999,"%s: Invalid input! \r\n",fname);
      break;
    }
    return 0;
  case COMPLEX :
    switch (CmplxB) {
    case REAL :
      /* A complex, B real : complexify B */
      C2F(complexify)((X=2,&X));
      ret = C2F(intzgesv4)("slash",5L);
      break;
    case COMPLEX :
      /* A complex, B complex */
      ret = C2F(intzgesv4)("slash",5L);
      break;
    default:
      Scierror(999,"%s: Invalid input! \r\n",fname);
      break;
    }
    return 0;
  default :
    Scierror(999,"%s: Invalid input! \r\n",fname);
    return 0;
  }
  return 0;
}


int C2F(intbackslash)(fname)
  char *fname;
{
  int *header1;int *header2;
  int CmplxA;int CmplxB;int ret;int X;

  extern int C2F(intdgesv3) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzgesv3) __PARAMS((char *fname, unsigned long fname_len));

  /*   backslash(A,B)  */
  header1 = (int *) GetData(1);    header2 = (int *) GetData(2);
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
      Scierror(999,"%s: Invalid input! \r\n",fname);
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
      Scierror(999,"%s: Invalid input! \r\n",fname);
      break;
    }
    return 0;
  default :
    Scierror(999,"%s: Invalid input! \r\n",fname);
    return 0;
    break;
  }
}
 
int C2F(intschur)(fname)
     char *fname;

{
  int *header1;int *header2;int *header3; 
  int Cmplx, CmplxA, CmplxB;
  int ret, something, X;
  int which = 1; int longueur;

  extern int C2F(intdgees0) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzgees0) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intoschur) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzschur) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intdgees1) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzgees1) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intfschur) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzfschur) __PARAMS((char *fname, unsigned long fname_len));

  extern int C2F(intdgges) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzgges) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intogschur) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intozgschur) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzgschur) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intgschur) __PARAMS((char *fname, unsigned long fname_len));

  extern int schtst __PARAMS((int longueur, int *header));
  if (GetType(1)!=1) {
    OverLoad(1);
    return 0;
  }
  header1 = (int *) GetData(1);   CmplxA=header1[3];
  switch (Rhs) {
  case 1:   /* schur(A)   */
    switch (CmplxA) {
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
    break;
  case 2 :   /* schur(A, something)   */
    header2 = (int *) GetData(2); something=header2[0];
    switch (something) {
    case FUNCTION :
      switch (CmplxA) {
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
	switch (CmplxA) {
	case REAL:
	  ret = C2F(intdgees1)("schur",5L);
	  break;
	case COMPLEX:
	  Scierror(999,"%s: Invalid call! \r\n",fname);
	  break;
	}
	return 0;
      case STRINGCOMPLEX:
	switch (CmplxA) {
	case REAL:
	  ret = C2F(intdgees1)("schur",5L);
	  break;
	case COMPLEX:
	  ret = C2F(intzgees1)("schur",5L);
	  break;
	}
	break;
      default:
	/*   String is an external function  */
	switch (CmplxA) {
	case REAL:
	  ret = C2F(intfschur)("schur",5L);
	  break;
	case COMPLEX:
	  ret = C2F(intzfschur)("schur",5L);
	  break;
	}
      }
      break;
    case DOUBLE: /*schur(A,B)*/
      if (GetType(2)!=1) {
	OverLoad(2);
	return 0;
      }
      CmplxB=header2[3];
      if ((CmplxA == 0) && (CmplxB ==0))  Cmplx =0; 
      else if ((CmplxA == 1) && (CmplxB ==0)) {
	C2F(complexify)((X=2,&X));
	Cmplx=1;}
      else if ((CmplxA == 0) && (CmplxB ==1)) {
	C2F(complexify)((X=1,&X));
	Cmplx=1;}
      else  Cmplx=1;

      switch (Cmplx) {
      case REAL :
	ret = C2F(intdgges)("schur",6L);
	break;
      case COMPLEX :
	ret = C2F(intzgges)("schur",6L);
	break;
      default :
	Scierror(999,"%s: Invalid input! \r\n",fname);
	break;
      }
      break;
    } /* end of switch (something) */
    break;
  case 3: /* schur(A,B,something) */
    if (GetType(2)!=1) {
      OverLoad(2);
      return 0;
    }
    header2 = (int *) GetData(2); something=header2[0];
    CmplxB=header2[3];
    if ((CmplxA == 0) && (CmplxB ==0))  Cmplx =0; 
    else if ((CmplxA == 1) && (CmplxB ==0)) {
      C2F(complexify)((X=2,&X));
      Cmplx=1;}
    else if ((CmplxA == 0) && (CmplxB ==1)) {
      C2F(complexify)((X=1,&X));
      Cmplx=1;}
    else  Cmplx=1;

    header3 = (int *) GetData(3); something=header3[0];
    switch (something) {
    case FUNCTION :
      switch (Cmplx) {
      case REAL :
	ret = C2F(intogschur)("schur",6L);
	break;
      case COMPLEX :
	ret = C2F(intozgschur)("schur",6L);
	break;
      default :
	Scierror(999,"%s: Invalid input! \r\n",fname);
	break;
      }
      break;
    case STRING  :
      switch (Cmplx) {
      case REAL:
	ret = C2F(intgschur)("schur",6L);
	break;
      case COMPLEX:
	ret = C2F(intzgschur)("schur",6L);
	break;
      default :
	Scierror(999,"%s: Invalid input! \r\n",fname);
	break;
      }
      break;
    default: /* switch (something) */
      break;
    }
    break; /* end of case 3 */
  default : 
    Scierror(999,"%s: Invalid call! \r\n",fname);
    break;
  } /*end of switch(Rhs) */
return 0;
}

int C2F(intbalanc)(fname)
  char *fname;
{
  int *header1, *header2;
  int CmplxA, CmplxB;
  int ret;int X;

  extern int C2F(intdgebal) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzgebal) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intdggbal) __PARAMS((char *fname, unsigned long fname_len));
  extern int C2F(intzggbal) __PARAMS((char *fname, unsigned long fname_len));

  switch (Rhs) {
  case 1:   /* balanc(A)   */
    if (GetType(1)!=1) {
      OverLoad(1);
      return 0;
    }
    header1 = (int *) GetData(1);    
    CmplxA=header1[3];
    switch (CmplxA) {
    case REAL:
      ret = C2F(intdgebal)("balanc",6L);
      break;
    case COMPLEX:
      ret = C2F(intzgebal)("balanc",6L);
      break;
    default:
      Scierror(999,"%s: Invalid input! \r\n",fname);
      break;
    } /* end switch  (CmplxA) */
    break; /* end case 1 */
  case 2: /* balanc(A,B) */
    if (GetType(1)!=1) {
      OverLoad(1);
      return 0;
    }
    if (GetType(2)!=1) {
      OverLoad(2);
      return 0;
    }
    header1 = (int *) GetData(1);    
    header2 = (int *) GetData(2);
    CmplxA=header1[3];   
    CmplxB=header2[3];
    switch (CmplxA) {
    case REAL:   
      switch (CmplxB) {
      case REAL :
	/* A real, Breal */
	ret = C2F(intdggbal)("balanc",6L);
	break;
      case COMPLEX :
	/* A real, B complex : complexify A */
	C2F(complexify)((X=1,&X));
	ret = C2F(intzggbal)("balanc",6L);
	break;
      default:
	Scierror(999,"%s: Invalid input! \r\n",fname);
	break;
      }
      break;
    case COMPLEX :
      switch (CmplxB) {
      case REAL :
	/* A complex, B real : complexify B */
	C2F(complexify)((X=2,&X));
	ret = C2F(intzggbal)("balanc",6L);
	break;
      case COMPLEX :
	/* A complex, B complex */
	ret = C2F(intzggbal)("balanc",6L);
	return 0;
	break;
      default:
	Scierror(999,"%s: Invalid input! \r\n",fname);
	break;
      }
      break;
    default :
      Scierror(999,"%s: Invalid input! \r\n",fname);
      break;
    } /*end  switch (CmplxA) */
    break;/* end case 2 */
  }/* end switch (Rhs) */
  return 0;
}

int schtst (longueur, header)
     int longueur;
     int *header;
{
  if( (longueur==1) && ( Abs(header[6])==27 ) ) return STRINGREAL;  /* "r" */

  if( (longueur==4) && ( Abs(header[6])==27 ) && (Abs(header[7])==14 ) && (Abs(header[8])==10 ) && ( Abs(header[9])==21 ) ) return STRINGREAL;  /* "real"  */
  if( (longueur==4) && ( Abs(header[6])==12 ) && (Abs(header[7])==24 ) && (Abs(header[8])==22 ) && ( Abs(header[9])==25 ) ) return STRINGCOMPLEX;  /* "comp" */
  if( (longueur==7) && ( Abs(header[6])==12 ) && (Abs(header[7])==24 ) && (Abs(header[8])==22 ) && ( Abs(header[9])==25 ) && ( Abs(header[10])==21) && ( Abs(header[11])==14) && ( Abs(header[12])==33) ) return STRINGCOMPLEX;  /* "complex" */
return 0;
}

typedef int (*des_interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct {
  des_interf f;    /** function **/
  char *name;      /** its name **/
} LapackTable;

 
static LapackTable Tab[]={
  {C2F(intqr),"qr"},
  {C2F(intsvd),"svd"},
  {C2F(intlsq),"lsq"},
  {C2F(inteig),"spec"},
  {C2F(intinv),"inv"},
  {C2F(intrcond),"rcond"},
  {C2F(intchol),"chol"},
  {C2F(intlu),"lu"},
  {C2F(intslash),"slash"},
  {C2F(intbackslash),"backslash"},
  {C2F(intschur),"schur"},
  {C2F(inthess),"hess"},
  {C2F(intdet),"det"},
  {C2F(intbalanc),"balanc"},
};

int C2F(intlapack)()
{  
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
  C2F(putlhsvar)();
  return 0;
}
