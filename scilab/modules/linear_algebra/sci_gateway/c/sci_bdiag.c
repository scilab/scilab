
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "api_common.h"
#include "api_double.h"

#include "MALLOC.h"
#include "stack-c.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "localization.h"
#include "core_math.h" /* for Abs  macro */

extern int C2F(vfinite)(int const*n, double const*v);

extern void C2F(bdiag)(int const * lda, int const* n, double* a, double const * epsshr, double const* rMax, double* er, double* ei, int *bs
		       , double* x, double* xi, double* scale, int const* job, int * fail);
extern void C2F(wbdiag)(int const* lda, int const* n, double* ar, double* ai, double const*rMax, double* er, double* ei, int* bs
			, double* xr , double* xi, double* yr, double* yi, double* scale, int const* job, int* fail);

int C2F(intbdiagr)(char *fname, long unsigned int fname_len)
{
  int ret= 0;

  int* arg[2]= {NULL, NULL};

  double* pDataReal[2]={NULL, NULL};
  double* pDataImg[2]={NULL, NULL};
 
  int iCols[2]={0, 0}, iRows[2]= {0, 0};
  int i;

  /* /!\ TODO: original C code does not perform overload dispatching here, check that this is not an oversight */
  CheckRhs(1,2);
  CheckLhs(1,3);
  for(i=0; i != Rhs; ++i)
    {
      getVarAddressFromPosition(i+1, &arg[i]);
      if(getVarType(arg[i])!= sci_matrix)
	{
	  Scierror(201,_("%s: Wrong type for argument %d: Real or complex matrix expected.\n"),fname, i +1);
	  ret= 201;
	}
      if(isVarComplex(arg[i] ))
	{
	  GetRhsVarMatrixComplex(i+1, &iRows[i], &iCols[i], &pDataReal[i], &pDataImg[i ]);
	}
      else
	{
	  GetRhsVarMatrixDouble(i+1, &iRows[i], &iCols[i], &pDataReal[i]);
	}
    }
  if (iCols[0] != iRows[0])
    {
      Scierror(20, _("%s: Wrong type for input argument #%d: Square matrix expected.\n"), fname, 1);
      ret= 20;
    }
  if((Rhs == 2) && ((iRows[1] != 1) || (iCols[1] != 1)))
    {
      Scierror(999,_("%s: Wrong size for input argument #%d: Scalar expected.\n"), fname, 2);
      ret= 999;
    }
  if(!ret)
    {
      if (iCols[0] == 0) /* && iRows[0] == 0 because  we checked that the matrix is square */
	{
	  double* dummy;
	  allocMatrixOfDouble(Rhs+1, 0, 0, &dummy);
	  allocMatrixOfDouble(Rhs+2, 0, 0, &dummy);
	  LhsVar(1)= 1; /* original C code does not check for Lhs, so neither do I */
	  LhsVar(2)= Rhs+1;
	  LhsVar(3)= Rhs+2;
	}
      else
	{	
	  int const totalSize= iRows[0] * iCols[0];
	  if (  ! (C2F(vfinite)(&totalSize, pDataReal[0]) &&(!isVarComplex(arg[0]) || (C2F(vfinite)(&totalSize, pDataImg[0])))))
	    {
	      Scierror(264,_("Wrong value for argument %d: Must not contain NaN or Inf.\n"),1);
	      ret= 264;;
	    }
	  if(!ret)
	    {
	      double rMax;
	      if(Rhs==2)
		{
		  rMax= *pDataReal[1];
		}
	      else
		{
		  int j;
		  for (j = 0; j != iCols[0]; ++j) 
		    {
		      double t = 0.;
		      for (i = 0; i != iCols[0]; ++i) 
			{
			  t += Abs(pDataReal[0][i+j*iCols[0] ]);
			}
		      rMax= Max(t, rMax);
		    }
		}
	      {
		double* lxr;
		double* lxi;
		double* le;
		int* lib;
		double* lw;
		int stackAllocError= 0;
		if(iIsComplex(1))
		  {
		    stackAllocError= iAllocComplexMatrixOfDouble(Rhs+1, iCols[0], iCols[0], &lxr, &lxi);
		  }
		else
		  {
		    stackAllocError= iAllocMatrixOfDouble(Rhs+1, iCols[0], iCols[0], &lxr);
		    lxi=lxr+iCols[0]*iCols[0];
		  }
	      /* allocating the two memory buffers in one place as the original code did */
	      le= (double*) MALLOC( 2*iCols[0] * sizeof(double) ); 
	      lib= (int*) MALLOC(iCols[0] * sizeof(int));
	      lw= (double*)MALLOC(iCols[0] * sizeof(double));
	      if(le && lib && lw && !stackAllocError)
		{
		  int fail;
		  int const job=0;
		  if(isVarComplex(arg[0]))
		    {
		      double dummy;
		      C2F(wbdiag)(&iCols[0], &iCols[0], pDataReal[0], pDataImg[0], &rMax, le, le + iCols[0], lib, lxr , lxi,  &dummy, &dummy
				  , lw, &job, &fail);

		    }
		  else
		    {      /*     subroutine bdiag(lda,n,a,epsshr,rMax,er,ei,bs,x,xi,scale,job,fail) */
		      double const epsshr= 0.;
		      C2F(bdiag)(&iCols[0], &iCols[0], pDataReal[0], &epsshr, &rMax, le, le + iCols[0], lib, lxr, lxi, lw, &job, &fail);
		    }
		  if (fail) 
		    {
		      Scierror(24,_("%s: Non convergence in QR steps.\n"),fname);
		      ret= 24;
		    }
		  else
		    {
		      LhsVar(1) = 1;
		      LhsVar(2) = Rhs+1;
		      if (Lhs == 3) 
			{
			  double* lbs;
			  int k, nbloc = 0;
			  for (k = 0; k != iCols[0]; ++k)
			    {
			      nbloc+= (lib[k]>=0) ? 1: 0 ;
			    }
			  allocMatrixOfDouble(Rhs + 2, nbloc, 1, &lbs);
			  for (i = k = 0; k != iCols[0]; ++k) 
			    {
			      if(lib[k] >= 0)
				{
				  lbs[i]=(double) lib[k];
				  ++i;
				}
			    }
			  LhsVar(3) = Rhs+2;
			}
		    }
		}
	      FREE(le);
	      FREE(lib);
	      FREE(lw);
	      }
	    }
	}
    }
  return ret;
}
