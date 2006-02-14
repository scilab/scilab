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
extern void GlobalSortchar(char * *a,int *ind,int flag,int n,int p,char dir);
extern void LexiColchar(char * *a,int *ind,int flag,int n,int p,char dir);
extern void LexiRowchar(char * *a,int *ind,int flag,int n,int p,char dir);
extern void RowSortchar(char * *a,int *ind,int flag,int n,int p,char dir);
extern void ColSortchar(char * *a,int *ind,int flag,int n,int p,char dir);
extern void GlobalSortdouble(double *a,int *ind,int flag,int n,int p,char dir);
extern void LexiColint(int *a,int *ind,int flag,int n,int p,char dir);
extern void LexiRowint(int *a,int *ind,int flag,int n,int p,char dir);
extern void RowSortdouble(double *a,int *ind,int flag,int n,int p,char dir);
extern void ColSortdouble(double *a,int *ind,int flag,int n,int p,char dir);
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
int C2F(gsort)(int *xI, double *xD, int *ind, int *iflag, int *m, int *n, char *type, char *iord)
{

  switch ( type[0])
    {
    case 'r' :  ColSortdouble(xD,ind,*iflag,*m,*n,iord[0]);break;
    case 'c' :  RowSortdouble(xD,ind,*iflag,*m,*n,iord[0]);break;
    case 'l' :  
      if ( type[1] == 'r' ) 
	CNAME(LexiRow,int)(xI,ind,*iflag,*m,*n,iord[0]);
      else
	CNAME(LexiCol,int)(xI,ind,*iflag,*m,*n,iord[0]);
      break;
    case 'g' : 
    default :  GlobalSortdouble(xD,ind,*iflag,*m,*n,iord[0]);break;
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
    case 'r' :  ColSortchar(data,ind,*iflag,*m,*n,iord[0]);break;
    case 'c' :  RowSortchar(data,ind,*iflag,*m,*n,iord[0]);break;
    case 'l' :  
      if ( type[1] == 'r' ) 
				LexiRowchar(data,ind,*iflag,*m,*n,iord[0]);
      else
				LexiColchar(data,ind,*iflag,*m,*n,iord[0]);
      break;
    case 'g' : 
    default :  GlobalSortchar(data,ind,*iflag,*m,*n,iord[0]);break;
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
    case 'r' :  ColSortchar(data,ind,*iflag,*m,*n,iord[0]);break;
    case 'c' :  RowSortchar(data,ind,*iflag,*m,*n,iord[0]);break;
    case 'l' :  
      if ( type[1] == 'r' ) 
		LexiRowchar(data,ind,*iflag,*m,*n,iord[0]);
      else
		LexiColchar(data,ind,*iflag,*m,*n,iord[0]);
      break;
    case 'g' : 
    default :  GlobalSortchar(data,ind,*iflag,*m,*n,iord[0]);break;
    }
}
/*-----------------------------------------------------------------------------------*/ 
