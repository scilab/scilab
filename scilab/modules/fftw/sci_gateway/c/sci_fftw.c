/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006/2007 - INRIA - Alan LAYEC
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "sci_fftw.h"
#include "fftw_utilities.h"
#include "callfftw.h"
#include "MALLOC.h"
#include "gw_fftw.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern unsigned cur_fftw_flags;
/*--------------------------------------------------------------------------*/

/* fftw function.
*
* Scilab Calling sequence :
*   -->fftw(a,sign,dim,incr);
*
* Input : rhs(1) : a scilab double complex or real
*                  vector (row or column)/matrix
*
*         rhs(2) : a scilab double or integer
*                  scalar (-1 or 1) giving the sign
*                  in the exponential component
*
*         rhs(3) : a scilab double or integer
*                  vector (row or column) of dimension
*                  of the Fast Fourier Transform to perform
*
*         rhs(4) : a scilab double or integer
*                  scalar...
*
* Output : a scilab double complex or real
*          vector(row or column)/matrix that
*          gives the result of the transform.
*
*/
int sci_fftw(char *fname,unsigned long fname_len)
{
  /* declaration of variables to store scilab parameters address */
  static int lr1 = 0,li1 = 0,it1 = 0,m1 = 0, n1 = 0; /* Rhs(1) */

  static int it2 = 0,m2 = 0,n2 = 0;   /* Rhs(2) */

  static int it3 = 0,m3 = 0,n3 = 0;           /* Rhs(3) */
  int mn3 = 0;

  static int it4 = 0,m4 = 0,n4 = 0;           /* Rhs(4) */
  int mn4 = 0;

  int *header = NULL;

  double         *ptr_d  = NULL;
  char           *ptr_c  = NULL;
  unsigned char  *ptr_uc = NULL;
  short          *ptr_s  = NULL;
  unsigned short *ptr_us = NULL;
  int            *ptr_i  = NULL;
  unsigned int   *ptr_ui = NULL;

  /* specific declaration for FFTW library variable */
  fftw_plan p;
  guru_dim_struct gdim;

  /* input/output address for transform variables */
  double *ri = NULL,*ii = NULL,*ro = NULL,*io = NULL;
  double *ptr = NULL;

  /* local counter variable */
  int i = 0,j = 0,k = 0;

  /* local variable */
  int isn = 0,vect = 0;
  int *n = NULL,*nspn = NULL;
  double zero=0.0;

  /****************************************
   * Basic constraints on rhs arguments  *
   ****************************************/

  /* check min/max lhs/rhs arguments of scilab funcion */
  CheckRhs(1,4);
  CheckLhs(1,1);

  /* 3 rhs not allowed */
  if (Rhs == 3) {
	  Scierror(39,_("%s: Wrong number of input arguments: %d not expected.\n"),fname,3);
   return(0);
  }

  /* retrieve address of input variable to transform */
  GetRhsCVar(1,MATRIX_OF_DOUBLE_DATATYPE, &it1, &m1, &n1, &lr1, &li1);

  /* void input gives void output */
  if ((m1<1)|(n1<1)) {
   LhsVar(1) =  1;
   PutLhsVar();
   return(0);
  }

  /******************************************
   * Advanced constraints on rhs arguments *
   * Get value of rhs arguments            *
   ******************************************/

  /*only one rhs argument*/
  /*forward fft*/
  if (Rhs==1) {
   /* sign of the exp. component */
   isn=-1;
  }
  /* two or four rhs arguments case */
  else {
   /* Get dim/type of Rhs(2) */
   header = GetData(2);
   m2  = header[1];
   n2  = header[2];
   it2 = header[3];

   /* */
   CheckDims(2,m2,n2,1,1);

   /* look at for type of Rhs(2) */
   if (VarType(2)==sci_ints) { /* int */
    /* */
	   switch (it2) /* Types defines in stack-c.h */
    {
     case I_CHAR   : ptr_c=IC_CHAR(&header[4]);
                     isn=(int) ptr_c[0];
                     break;
     case I_INT16  : ptr_s=IC_INT16(&header[4]);
                     isn=(int) ptr_s[0];
                     break;
     case I_INT32  : ptr_i=IC_INT32(&header[4]);
                     isn=(int) ptr_i[0];
                     break;
     case I_UCHAR  : ptr_uc=IC_UCHAR(&header[4]);
                     isn=(int) ptr_uc[0];
                     break;
     case I_UINT16 : ptr_us=IC_UINT16(&header[4]);
                     isn=(int) ptr_us[0];
                     break;
     case I_UINT32 : ptr_ui=IC_UINT32(&header[4]);
                     isn=(int) ptr_ui[0];
                     break;
    }
   }
   else if (VarType(2)==sci_matrix) { /* double */
    ptr_d=(double *)(&header[4]);
    isn=(int)ptr_d[0];
   }
   /* if is not int/double then error message */
   else {
	Scierror(53,_("%s: Wrong type for input argument #%d: A Real or Complex expected.\n"),fname,2);
    return(0);
   }

   /* check value of second rhs argument */
   if ((isn!=1)&&(isn!=-1)) 
   {
	Scierror(53,_("%s: Wrong values for input argument #%d.\n"), fname,2);
    return(0);
   }

   /* four rhs arguments */
   if (Rhs==4) {
    /* Get dim/type of Rhs(3) */
    header = GetData(3);
    m3  = header[1];
    n3  = header[2];
    it3 = header[3];

    /* look at for type of Rhs(3) */
    if ((VarType(3)!=sci_ints)&&(VarType(3)!=sci_matrix)) 
	{
		Scierror(53,_("%s: Wrong type for input argument #%d: N-dimensionnal array expected.\n"),fname,3);
		return(0);
    }

    /* */
    mn3=m3*n3;

    /* check dims */
    if (m3*n3==0) 
	{
		Scierror(999,_("%s: Wrong size for input argument #%d.\n"),fname,3);
		return(0);
    }

    /* Get dim/type of Rhs(4) */
    header = GetData(4);
    m4  = header[1];
    n4  = header[2];
    it4 = header[3];

    /* look at for type of Rhs(4) */
    if ((VarType(4)!=sci_ints)&&(VarType(4)!=sci_matrix)) 
	{ 
		/* int */
		Scierror(53,_("%s: Wrong type for input argument #%d: int matrix expected.\n"),fname,3);
		return(0);
    }

    /* */
    mn4=m4*n4;

    /* check dims */
    if (m4*n4 == 0) 
	{
		Scierror(999,_("%s: Wrong size for input argument #%d.\n"), fname,4);
		return(0);
    }

    /* cross variable size checking */
    if (mn4 != mn3) 
	{
		Scierror(999,_("%s: Incompatible input arguments '#%d and '#%d': Same sizes expected.\n"),fname,3,4);
		return(0);
    }

    /* alloc n/nspn with MALLOC */
    if ((n=(int *)MALLOC(mn4*sizeof(int)))==NULL) 
	{
		Scierror(999,_("%s: No more memory.\n"),fname);
		return(0);
    }

    if ((nspn=(int *)MALLOC(mn4*sizeof(int)))==NULL) 
	{
		Scierror(999,_("%s: No more memory.\n"),fname);
		FREE(n);
		return(0);
    }

    /* n    <- Rhs(3) */
    /* nspn <- Rhs(4) */
    /* check values   */
    for (i=0;i<mn3;i++) {
     /* Rhs(3) */
     header = GetData(3);

     switch (VarType(3))
     {
      /* double */
      case 1 :
      {
       ptr_d=(double *)(&header[4]);
       n[i]=(int)ptr_d[i];
       break;
      }
      /* int */
      case 8 :
      {
       switch (it3)
       {
        case I_CHAR   : ptr_c=IC_CHAR(&header[4]);
                        n[i]=(int) ptr_c[i];
                        break;

        case I_INT16  : ptr_s=IC_INT16(&header[4]);
                        n[i]=(int) ptr_s[i];
                        break;

        case I_INT32  : ptr_i=IC_INT32(&header[4]);
                        n[i]=(int) ptr_i[i];
                        break;

        case I_UCHAR  : ptr_uc=IC_UCHAR(&header[4]);
                        n[i]=(int) ptr_uc[i];
                        break;

        case I_UINT16 : ptr_us=IC_UINT16(&header[4]);
                        n[i]=(int) ptr_us[i];
                        break;

        case I_UINT32 : ptr_ui=IC_UINT32(&header[4]);
                        n[i]=(int) ptr_ui[i];
                        break;
       }
       break;
      }
     }
     /* check value of n[i] */
     if (n[i]<=0) 
	 {
		Scierror(999,_("%s: Wrong values for input argument #%d: Non-negative integers expected.\n"),fname,3);
		FREE(n);FREE(nspn);
		return(0);
		break;
     }

     /* Rhs(4) */
     header = GetData(4);

     switch (VarType(4))
     {
      /* double */
      case 1 :
      {
       ptr_d=(double *)(&header[4]);
       nspn[i]=(int)ptr_d[i];
       break;
      }
      /* int */
      case 8 :
      {
       switch (it4)
       {
        case I_CHAR   : ptr_c=IC_CHAR(&header[4]);
                        nspn[i]=(int) ptr_c[i];
                        break;

        case I_INT16  : ptr_s=IC_INT16(&header[4]);
                        nspn[i]=(int) ptr_s[i];
                        break;

        case I_INT32  : ptr_i=IC_INT32(&header[4]);
                        nspn[i]=(int) ptr_i[i];
                        break;

        case I_UCHAR  : ptr_uc=IC_UCHAR(&header[4]);
                        nspn[i]=(int) ptr_uc[i];
                        break;

        case I_UINT16 : ptr_us=IC_UINT16(&header[4]);
                        nspn[i]=(int) ptr_us[i];
                        break;

        case I_UINT32 : ptr_ui=IC_UINT32(&header[4]);
                        nspn[i]=(int) ptr_ui[i];
                        break;
       }
       break;
      }
     }
     /* check value of nspn[i] */
     if (nspn[i]<=0) 
	 {
		Scierror(999,_("%s: Wrong values for input argument #%d: Non-negative integers expected.\n"),fname,4);
		FREE(n);FREE(nspn);
		return(0);
		break;
     }

    }
   }
  }

  /*********************************************
   * Set address of input/ouput array for fftw *
   *********************************************/

  /* complex data */
  if (it1==1) {
   /* set input array address */
   ri=stk(lr1);
   ii=stk(li1);
  }
  /* real data */
  else if (it1==0) {
   /* set input array address */
   CreateCVar(1,MATRIX_OF_DOUBLE_DATATYPE, (i=1,&i), &m1, &n1, &lr1,&li1);

   ri=stk(lr1);
   ii=stk(li1);

   /* set all elements of imaginary parts to 0 */
   C2F(dset)((i=m1*n1,&i), &zero, ii, (k=1,&k));
  }

  /* set output array address */
   ro=ri;
   io=ii;

  /* reverse address of input/output array
   * if it is a backward fft
   */
  if (isn==1) {
   /* reverse input */
   ptr=ri;
   ri=ii;
   ii=ptr;

   /* reverse output */
   ptr=ro;
   ro=io;
   io=ptr;
  }

  /**********************************
   * Set arguments needed for fftw *
   **********************************/

  /* check if it is one column or row vector */
  vect = 0;
  if ((m1==1)|(n1==1)) vect = 1;

  /* vector transform */
  if ((vect)&&(Rhs<=2)) {
   /* set arguments of fftw_plan_guru_split_dft */
   gdim.rank=1;
   if ((gdim.dims=(fftw_iodim *)MALLOC(sizeof(fftw_iodim)))==NULL) 
   {
		Scierror(999,_("%s: No more memory.\n"),fname);
		return(0);
   }

   gdim.dims[0].n = m1*n1;
   gdim.dims[0].is = 1;
   gdim.dims[0].os = 1;

   gdim.howmany_rank=0;
   gdim.howmany_dims=NULL;
  }
  else {
   /* 2D fft */
   if (Rhs<=2) {
    /* set arguments of fftw_plan_guru_split_dft */
    gdim.rank=2;
    if ((gdim.dims=(fftw_iodim *)MALLOC(sizeof(fftw_iodim)*gdim.rank))==NULL) 
	{
		Scierror(999,_("%s: No more memory.\n"), fname);
		return(0);
    }

    gdim.dims[0].n = m1;
    gdim.dims[0].is = 1;
    gdim.dims[0].os = 1;

    gdim.dims[1].n = n1;
    gdim.dims[1].is = m1;
    gdim.dims[1].os = m1;

    gdim.howmany_rank=0;
    gdim.howmany_dims=NULL;
   }
   /* multidimensional fft */
   else if (Rhs==4) {
    /* size 1x1 for Rhs(3)/Rhs(4) */
    if (mn3==1) {
    /* **This is a special case**
     * all is made in the following block.
     * compatibility with scilab fft function.
     */

     /* set arguments of fftw_plan_guru_split_dft */
     gdim.rank = 1;
     if ((gdim.dims=(fftw_iodim *)MALLOC(sizeof(fftw_iodim)))==NULL) 
	 {
		Scierror(999,_("%s: No more memory.\n"),fname);
		return(0);
     }

     gdim.dims[0].n = n[0];
     gdim.dims[0].is = nspn[0];
     gdim.dims[0].os = nspn[0];

     gdim.howmany_rank = 1;
     if ((gdim.howmany_dims=(fftw_iodim *)MALLOC(sizeof(fftw_iodim)))==NULL) 
	 {
		Scierror(999,_("%s: No more memory.\n"), fname);
		return(0);
     }

     /* find number of transforms to compute */
     gdim.howmany_dims[0].n = 0;
     j = (n[0]-1)*nspn[0];
     while(j<((m1*n1)-(nspn[0]-1))) {
      gdim.howmany_dims[0].n++;
      j += nspn[0]*n[0];
     }
     if (j<=((m1*n1)-(nspn[0]-1)))
       k = j+nspn[0];
     else
       k = j-nspn[0]*n[0]+nspn[0];

     gdim.howmany_dims[0].is = n[0]*nspn[0];
     gdim.howmany_dims[0].os = n[0]*nspn[0];

     if ((p = GetFFTWPlan(&gdim,
                          ri, ii, ro, io,
                          cur_fftw_flags,isn)) == NULL)
	 {
		Scierror(999,_("%s: No more memory.\n"), fname);
		FREE(gdim.dims);
		FREE(gdim.howmany_dims);
		return(0);
     }
     else {
      if (isn==1) { /* backward */
       double ak;

       ak=1/((double)(n[0]));

       /* must find a BLAS/calelm function to do that */
       for(i=0;i<k;i++) {
        ri[i]=ri[i]*ak;
        ii[i]=ii[i]*ak;
       }
      }

      /* */
      for(i=0;i<nspn[0];i++)
        call_fftw_execute_split_dft(p,&ri[i],&ii[i],&ro[i],&io[i]);

      /* */
      FREE(gdim.dims);
      FREE(gdim.howmany_dims);
      FREE(n);FREE(nspn);

      LhsVar(1) = 1;
      PutLhsVar();

      return(0);
     }
    }

    /* size mxn Rhs(3)/Rhs(4)  */
    else {
      /* set arguments of fftw_plan_guru_split_dft */
      gdim.rank=mn3;
      if ((gdim.dims=(fftw_iodim *)MALLOC(sizeof(fftw_iodim)*gdim.rank))==NULL) 
	  {
		Scierror(999,_("%s: No more memory.\n"),fname);
		return(0);
      }

      /* */
      for(j=0;j<gdim.rank;j++) {
       gdim.dims[j].n = n[j];
       gdim.dims[j].is = nspn[j];
       gdim.dims[j].os = nspn[j];
      }

      gdim.howmany_rank=0;
      gdim.howmany_dims=NULL;
    }

   }
  }

  /*****************
   * Get fftw plan *
   *****************/

  /* call GetFFTWPlan */
  if ((p = GetFFTWPlan(&gdim,
                       ri, ii, ro, io,
                       cur_fftw_flags,isn)) == NULL) {
   Scierror(999,_("%s: No more memory.\n"),
                fname);
   FREE(gdim.dims);
   FREE(gdim.howmany_dims);
   return(0);
  }

  /* */
  FREE(gdim.dims);
  FREE(gdim.howmany_dims);

  /***************
   * Compute fft *
   ***************/

  if (isn==1) { /* backward */
   /* */
   double ak;

   if (Rhs==4) {
    ak = 1;
    for(i=0;i<mn3;i++) ak=ak*1/((double)(n[i]));
    FREE(n);FREE(nspn);
   }
   else {
    ak = 1/((double)(m1*n1));
   }

   /* must find a BLAS/calelm function to do that */
   for(i=0;i<m1*n1;i++) {
    ri[i]=ri[i]*ak;
    ii[i]=ii[i]*ak;
   }
  }

  /* execute FFTW plan */
  call_fftw_execute_split_dft(p,ri,ii,ro,io);

  /***********************************
   * Return results in lhs argument *
   ***********************************/

  LhsVar(1) = 1;

  PutLhsVar();

  return(0);
}
/*--------------------------------------------------------------------------*/ 
