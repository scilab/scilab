/*-----------------------------------------------------------------------------------*/
/* Written by  Jean-Philippe Chancelier Copyright (C) ENPC 1998-2001                 *
 * Modified (restructuration and bug fix) by Allan Cornet  INRIA 2006                */
/*-----------------------------------------------------------------------------------*/ 
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Math.h"

#include "../os_specific/men_Sutils.h"
/*-----------------------------------------------------------------------------------*/ 
extern void sciqsort();

extern void GlobalSortstring(char * *a,int *ind,int flag,int n,int p,char dir);
extern void LexiColstring(char * *a,int *ind,int flag,int n,int p,char dir);
extern void LexiRowstring(char * *a,int *ind,int flag,int n,int p,char dir);
extern void RowSortstring(char * *a,int *ind,int flag,int n,int p,char dir);
extern void ColSortstring(char * *a,int *ind,int flag,int n,int p,char dir);

extern void GlobalSortdouble(double *a,int *ind,int flag,int n,int p,char dir);
extern void RowSortdouble(double *a,int *ind,int flag,int n,int p,char dir);
extern void ColSortdouble(double *a,int *ind,int flag,int n,int p,char dir);
extern void LexiColdouble(double *a,int *ind,int flag,int n,int p,char dir);
extern void LexiRowdouble(double *a,int *ind,int flag,int n,int p,char dir);

extern void GlobalSortint(int *a,int *ind,int flag,int n,int p,char dir);
extern void RowSortint(int *a,int *ind,int flag,int n,int p,char dir);
extern void ColSortint(int *a,int *ind,int flag,int n,int p,char dir);
extern void LexiColint(int *a,int *ind,int flag,int n,int p,char dir);
extern void LexiRowint(int *a,int *ind,int flag,int n,int p,char dir);

extern void GlobalSortuint(unsigned int *a,int *ind,int flag,int n,int p,char dir);
extern void RowSortuint(unsigned int *a,int *ind,int flag,int n,int p,char dir);
extern void ColSortuint(unsigned int *a,int *ind,int flag,int n,int p,char dir);
extern void LexiColuint(unsigned int *a,int *ind,int flag,int n,int p,char dir);
extern void LexiRowuint(unsigned int *a,int *ind,int flag,int n,int p,char dir);

extern void GlobalSortshort(short *a,int *ind,int flag,int n,int p,char dir);
extern void RowSortshort(short *a,int *ind,int flag,int n,int p,char dir);
extern void ColSortshort(short *a,int *ind,int flag,int n,int p,char dir);
extern void LexiColshort(short *a,int *ind,int flag,int n,int p,char dir);
extern void LexiRowshort(short *a,int *ind,int flag,int n,int p,char dir);

extern void GlobalSortushort(unsigned short *a,int *ind,int flag,int n,int p,char dir);
extern void RowSortushort(unsigned short *a,int *ind,int flag,int n,int p,char dir);
extern void ColSortushort(unsigned short *a,int *ind,int flag,int n,int p,char dir);
extern void LexiColushort(unsigned short *a,int *ind,int flag,int n,int p,char dir);
extern void LexiRowushort(unsigned short *a,int *ind,int flag,int n,int p,char dir);

extern void GlobalSortchar(char *a,int *ind,int flag,int n,int p,char dir);
extern void RowSortchar(char *a,int *ind,int flag,int n,int p,char dir);
extern void ColSortchar(char *a,int *ind,int flag,int n,int p,char dir);
extern void LexiColchar(char *a,int *ind,int flag,int n,int p,char dir);
extern void LexiRowchar(char *a,int *ind,int flag,int n,int p,char dir);

extern void GlobalSortuchar(unsigned char *a,int *ind,int flag,int n,int p,char dir);
extern void RowSortuchar(unsigned char *a,int *ind,int flag,int n,int p,char dir);
extern void ColSortuchar(unsigned char *a,int *ind,int flag,int n,int p,char dir);
extern void LexiColuchar(unsigned char *a,int *ind,int flag,int n,int p,char dir);
extern void LexiRowuchar(unsigned char *a,int *ind,int flag,int n,int p,char dir);

/*-----------------------------------------------------------------------------------*/ 
/******************************************************
 * General sort routine for Scilab 
 * xI is the transmitted table to sort ( if table is int ) 
 * xD is the transmitted table to sort ( if table is double ) 
 * ind is the int table to store the permutation 
 *     (which is to be initialized and changed )
 * iflag == if 1 ind is to be computed if 0 ind is ignored 
 * m,n : matrix size 
 * type : the operation ( see the interface ) 
 * iord : 'i' or 'd' : increasind or decreasing sort 
 ******************************************************/
int C2F(gsortd)(double *xD, int *ind, int *iflag, int *m, int *n, char *type, char *iord)
{

  switch ( type[0])
    {
    case 'r' :  ColSortdouble(xD,ind,*iflag,*m,*n,iord[0]);break;
    case 'c' :  RowSortdouble(xD,ind,*iflag,*m,*n,iord[0]);break;
    case 'l' :  
      if ( type[1] == 'r' ) 
	CNAME(LexiRow,double)(xD,ind,*iflag,*m,*n,iord[0]);
      else
	CNAME(LexiCol,double)(xD,ind,*iflag,*m,*n,iord[0]);
      break;
    case 'g' : 
    default :  GlobalSortdouble(xD,ind,*iflag,*m,*n,iord[0]);break;
    }
  return(0);
}

int C2F(gsortint)(int *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord)
{

  switch ( type[0])
    {
    case 'r' :  ColSortint(xI,ind,*iflag,*m,*n,iord[0]);break;
    case 'c' :  RowSortint(xI,ind,*iflag,*m,*n,iord[0]);break;
    case 'l' :  
      if ( type[1] == 'r' ) 
	CNAME(LexiRow,int)(xI,ind,*iflag,*m,*n,iord[0]);
      else
	CNAME(LexiCol,int)(xI,ind,*iflag,*m,*n,iord[0]);
      break;
    case 'g' : 
    default :  GlobalSortint(xI,ind,*iflag,*m,*n,iord[0]);break;
    }
  return(0);
}

int C2F(gsortuint)(unsigned int *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord)
{

  switch ( type[0])
    {
    case 'r' :  ColSortuint(xI,ind,*iflag,*m,*n,iord[0]);break;
    case 'c' :  RowSortuint(xI,ind,*iflag,*m,*n,iord[0]);break;
    case 'l' :  
      if ( type[1] == 'r' ) 
	CNAME(LexiRow,uint)(xI,ind,*iflag,*m,*n,iord[0]);
      else
	CNAME(LexiCol,uint)(xI,ind,*iflag,*m,*n,iord[0]);
      break;
    case 'g' : 
    default :  GlobalSortuint(xI,ind,*iflag,*m,*n,iord[0]);break;
    }
  return(0);
}

int C2F(gsortshort)(short *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord)
{

  switch ( type[0])
    {
    case 'r' :  ColSortshort(xI,ind,*iflag,*m,*n,iord[0]);break;
    case 'c' :  RowSortshort(xI,ind,*iflag,*m,*n,iord[0]);break;
    case 'l' :  
      if ( type[1] == 'r' ) 
	CNAME(LexiRow,short)(xI,ind,*iflag,*m,*n,iord[0]);
      else
	CNAME(LexiCol,short)(xI,ind,*iflag,*m,*n,iord[0]);
      break;
    case 'g' : 
    default :  GlobalSortshort(xI,ind,*iflag,*m,*n,iord[0]);break;
    }
  return(0);
}

int C2F(gsortushort)(unsigned short *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord)
{

  switch ( type[0])
    {
    case 'r' :  ColSortushort(xI,ind,*iflag,*m,*n,iord[0]);break;
    case 'c' :  RowSortushort(xI,ind,*iflag,*m,*n,iord[0]);break;
    case 'l' :  
      if ( type[1] == 'r' ) 
	CNAME(LexiRow,ushort)(xI,ind,*iflag,*m,*n,iord[0]);
      else
	CNAME(LexiCol,ushort)(xI,ind,*iflag,*m,*n,iord[0]);
      break;
    case 'g' : 
    default :  GlobalSortushort(xI,ind,*iflag,*m,*n,iord[0]);break;
    }
  return(0);
}

int C2F(gsortchar)(char *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord)
{

  switch ( type[0])
    {
    case 'r' :  ColSortchar(xI,ind,*iflag,*m,*n,iord[0]);break;
    case 'c' :  RowSortchar(xI,ind,*iflag,*m,*n,iord[0]);break;
    case 'l' :  
      if ( type[1] == 'r' ) 
	CNAME(LexiRow,char)(xI,ind,*iflag,*m,*n,iord[0]);
      else
	CNAME(LexiCol,char)(xI,ind,*iflag,*m,*n,iord[0]);
      break;
    case 'g' : 
    default :  GlobalSortchar(xI,ind,*iflag,*m,*n,iord[0]);break;
    }
  return(0);
}

int C2F(gsortuchar)(unsigned char *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord)
{

  switch ( type[0])
    {
    case 'r' :  ColSortuchar(xI,ind,*iflag,*m,*n,iord[0]);break;
    case 'c' :  RowSortuchar(xI,ind,*iflag,*m,*n,iord[0]);break;
    case 'l' :  
      if ( type[1] == 'r' ) 
	CNAME(LexiRow,uchar)(xI,ind,*iflag,*m,*n,iord[0]);
      else
	CNAME(LexiCol,uchar)(xI,ind,*iflag,*m,*n,iord[0]);
      break;
    case 'g' : 
    default :  GlobalSortuchar(xI,ind,*iflag,*m,*n,iord[0]);break;
    }
  return(0);
}



/*-----------------------------------------------------------------------------------*/ 
/******************************************************
 * General sort routine for Scilab 
 * The version for Scilab strings 
 * iflag == if 1 ind is to be computed if 0 ind is ignored 
 * m,n : matrix size 
 * type : the operation ( see the interface ) 
 * iord : 'i' or 'd' : increasind or decreasing sort 
 ******************************************************/
void C2F(gsorts_old)(int *value, int *ptrv, int *m, int *n, int *res, int *ptrres, int *ierr, int *ind, int *iflag, char *type, char *iord)
{
  char **data;
  int i,nv;
  int maxchars= *ierr;
  *ierr=0;
  nv = *m*(*n);
  /* conversion of scilab characters into strings */
  ScilabMStr2CM(value,&nv,ptrv,&(data),ierr);
  if ( *ierr == 1) return;
  switch ( type[0])
    {
    case 'r' :  ColSortstring(data,ind,*iflag,*m,*n,iord[0]);break;
    case 'c' :  RowSortstring(data,ind,*iflag,*m,*n,iord[0]);break;
    case 'l' :  
      if ( type[1] == 'r' ) 
				LexiRowstring(data,ind,*iflag,*m,*n,iord[0]);
      else
				LexiColstring(data,ind,*iflag,*m,*n,iord[0]);
      break;
    case 'g' : 
    default :  GlobalSortstring(data,ind,*iflag,*m,*n,iord[0]);break;
    }
  ScilabCM2MStr(data,nv,res,ptrres,maxchars,ierr);
  for (i=0;i< nv ;i++) FREE(data[i]); FREE(data);
}
/*-----------------------------------------------------------------------------------*/ 
/******************************************************
 * General sort routine for Scilab strings 
 * iflag == if 1 ind is to be computed if 0 ind is ignored 
 * m,n : matrix size 
 * type : the operation ( see the interface ) 
 * iord : 'i' or 'd' : increasind or decreasing sort 
 ******************************************************/
void C2F(gsorts)(char **data, int *ind, int *iflag, int *m, int *n, char *type, char *iord)
{

  switch ( type[0])
    {
    case 'r' :  ColSortstring(data,ind,*iflag,*m,*n,iord[0]);break;
    case 'c' :  RowSortstring(data,ind,*iflag,*m,*n,iord[0]);break;
    case 'l' :  
      if ( type[1] == 'r' ) 
		LexiRowstring(data,ind,*iflag,*m,*n,iord[0]);
      else
		LexiColstring(data,ind,*iflag,*m,*n,iord[0]);
      break;
    case 'g' : 
    default :  GlobalSortstring(data,ind,*iflag,*m,*n,iord[0]);break;
    }
}
/*-----------------------------------------------------------------------------------*/ 
