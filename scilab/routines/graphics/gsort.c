/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Math.h"

#include "../sun/men_Sutils.h"

extern void sciqsort();

/** swapcode for indices : indices are integers **/

#define swapcodeind CNAME(swapcode,int)

#include "gsort-int.h"
#include "gsort-double.h"
#include "gsort-string.h"

#define TYPE double

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

int C2F(gsort)(xI,xD,ind,iflag,m,n,type,iord)
     int *xI,*ind;
     double *xD;
     int *m,*n,*iflag;
     char *type,*iord;
{
  switch ( type[0])
    {
    case 'r' :  CNAME(ColSort,double)(xD,ind,*iflag,*m,*n,iord[0]);break;
    case 'c' :  CNAME(RowSort,double)(xD,ind,*iflag,*m,*n,iord[0]);break;
    case 'l' :  
      if ( type[1] == 'r' ) 
	CNAME(LexiRow,int)(xI,ind,*iflag,*m,*n,iord[0]);
      else
	CNAME(LexiCol,int)(xI,ind,*iflag,*m,*n,iord[0]);
      break;
    case 'g' : 
    default :  CNAME(GlobalSort,double)(xD,ind,*iflag,*m,*n,iord[0]);break;
    }
  return(0);
}

/******************************************************
 * General sort routine for Scilab 
 * The version for Scilab strings 
 * iflag == if 1 ind is to be computed if 0 ind is ignored 
 * m,n : matrix size 
 * type : the operation ( see the interface ) 
 * iord : 'i' or 'd' : increasind or decreasing sort 
 ******************************************************/

#ifndef TEST 

void C2F(gsorts_old)(value,ptrv,m,n,res,ptrres,ierr,ind,iflag,type,iord)
     int *value,*ptrv,*m,*n,*res,*ptrres,*ierr;
     int *ind;
     int *iflag;
     char *type,*iord;     
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
    case 'r' :  CNAME(ColSort,char)(data,ind,*iflag,*m,*n,iord[0]);break;
    case 'c' :  CNAME(RowSort,char)(data,ind,*iflag,*m,*n,iord[0]);break;
    case 'l' :  
      if ( type[1] == 'r' ) 
	CNAME(LexiRow,char)(data,ind,*iflag,*m,*n,iord[0]);
      else
	CNAME(LexiCol,char)(data,ind,*iflag,*m,*n,iord[0]);
      break;
    case 'g' : 
    default :  CNAME(GlobalSort,char)(data,ind,*iflag,*m,*n,iord[0]);break;
    }
  ScilabCM2MStr(data,nv,res,ptrres,maxchars,ierr);
  for (i=0;i< nv ;i++) FREE(data[i]); FREE(data);
}
#endif 

/******************************************************
 * General sort routine for Scilab strings 
 * iflag == if 1 ind is to be computed if 0 ind is ignored 
 * m,n : matrix size 
 * type : the operation ( see the interface ) 
 * iord : 'i' or 'd' : increasind or decreasing sort 
 ******************************************************/

#ifndef TEST 

void C2F(gsorts)(data,ind,iflag,m,n,type,iord)
     char **data;
     int *m,*n,*ind,*iflag;
     char *type,*iord;     
{
  switch ( type[0])
    {
    case 'r' :  CNAME(ColSort,char)(data,ind,*iflag,*m,*n,iord[0]);break;
    case 'c' :  CNAME(RowSort,char)(data,ind,*iflag,*m,*n,iord[0]);break;
    case 'l' :  
      if ( type[1] == 'r' ) 
	CNAME(LexiRow,char)(data,ind,*iflag,*m,*n,iord[0]);
      else
	CNAME(LexiCol,char)(data,ind,*iflag,*m,*n,iord[0]);
      break;
    case 'g' : 
    default :  CNAME(GlobalSort,char)(data,ind,*iflag,*m,*n,iord[0]);break;
    }
}
#endif 


/******************************************************
 * TEST 
 ******************************************************/

#ifdef TEST 


main()
{
  printf("Test for char \n");
  CNAME(sorttest,char)() ;
  printf("Test for int \n");
  CNAME(sorttest,int)() ;
  printf("Test for double \n");
  CNAME(sorttest,double)() ;
  return(0);
}
#endif



