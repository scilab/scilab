
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA 2007 - Cong WU
 * Copyright (C) INRIA 2008 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* desc : For usual or polynomial matrix  n  is the int equal to
   number of rows times number of columns of  M . (Also valid for  M
   a boolean matrix)

   For matrices made of character strings (and in particular for a
   character string)  length  returns in  n  the length of entries of
   the matrix of character strings  M .

   The length of a list is the number of elements in the list
   (also given by  size ).

   length('123')  is  3 .  length([1,2;3,4])  is  4 .                     */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "stack-c.h"
#include "api_common.h"
#include "api_string.h"
#include "api_double.h"
#include "core_math.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "charEncoding.h"
/*----------------------------------------------------------------------------*/
/* get length */
static int lengthStrings(int RhsPosition);
static int lengthOthers(char *fname);
static int lengthfunction(int RhsPosition,char *VariableType);
static int lengthPoly(int RhsPosition);
/* !!! WARNING !!! : Read comments about length on sparse matrix */
static int lengthSparse(int RhsPosition);
/*----------------------------------------------------------------------------*/
int sci_length(char *fname,unsigned long fname_len)
{
	int lenghtValue = 0;
	int m_out = 1, n_out = 1, l_out = 0;

	CheckRhs(1,1);
	CheckLhs(1,1);

	switch ( GetType(1) )
	{
		case sci_poly :
			lenghtValue = lengthPoly(1);
		break;

		case sci_strings :
			/* to optimize output, we write directly on stack */
			return lengthStrings(1);
		break;

		case sci_matrix :
			lenghtValue = lengthfunction(1,MATRIX_OF_DOUBLE_DATATYPE);
		break;

		case sci_boolean :
			lenghtValue = lengthfunction(1,MATRIX_OF_BOOLEAN_DATATYPE);
		break;

		case sci_sparse :
			lenghtValue = lengthSparse(1);
		break;

		case sci_ints :
			lenghtValue = lengthfunction(1,MATRIX_OF_INTEGER_DATATYPE);
		break;

		case sci_handles :
			lenghtValue = lengthfunction(1,GRAPHICAL_HANDLE_DATATYPE);
		break;

		case sci_list :
			lenghtValue = lengthfunction(1,LIST_DATATYPE);
		break;

		case sci_tlist :
			lenghtValue = lengthfunction(1,TYPED_LIST_DATATYPE);
		break;

		case sci_mlist :
			lenghtValue = lengthfunction(1,MATRIX_ORIENTED_TYPED_LIST_DATATYPE);
		break;

		case sci_lufact_pointer :
			lenghtValue = lengthfunction(1,SCILAB_POINTER_DATATYPE);
		default :
			return lengthOthers(fname);
		break;
	}

	CreateVar( Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &m_out,&n_out, &l_out);
	stk(l_out)[0] = (double)(lenghtValue);
	LhsVar(1) = Rhs+1 ;
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
static int lengthStrings(int RhsPosition)
{
	int m1 = 0, n1 = 0;
	int *piAddressVarOne = NULL;
	wchar_t **pStVarOne = NULL;
	int *lenStVarOne = NULL;

	int m_out = 0, n_out = 0;
	int *piAddressOut = NULL;
	double *pdOut = NULL;
	int i = 0;

	int ierr = 0;
	
	/* get Address of inputs */
	getVarAddressFromPosition(RhsPosition, &piAddressVarOne);

	if ( getVarType(piAddressVarOne) != sci_strings )
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),"length",1);
		return 0;
	}

	ierr = getMatrixOfWideString(piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
	if (ierr)
	{
		Scierror(999,_("%s: impossible to get dimensions of this matrix.\n"),"length");
		return 0;
	}

	lenStVarOne = (int*)MALLOC(sizeof(int) * (m1*n1));
	if (lenStVarOne == NULL)
	{
		Scierror(999,_("%s: No more memory.\n"),"length");
		return 0;
	}

	ierr = getMatrixOfWideString(piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
	if (ierr)
	{
		Scierror(999,_("%s: impossible to get lengths of this matrix.\n"),"length");
		return 0;
	}

	m_out = m1;  n_out = n1;
	pdOut = (double*)MALLOC(sizeof(double) * (m_out * n_out));
	if (pdOut == NULL)
	{
		FREE(lenStVarOne); lenStVarOne = NULL;
		Scierror(999,_("%s: No more memory.\n"),"length");
		return 0;
	}

	/* Convert to double for compatibility :'( */
	for (i = 0; i < m_out * n_out; i++)
	{
		pdOut[i] = (double)lenStVarOne[i];
	}
	FREE(lenStVarOne); lenStVarOne = NULL;

	createMatrixOfDouble(Rhs + 1, m_out, n_out, pdOut);
	LhsVar(1) = Rhs + 1; 
	C2F(putlhsvar)();

	FREE(pdOut); pdOut = NULL;
	return 0;
}
/*--------------------------------------------------------------------------*/
static int lengthOthers(char *fname)
{
	/* unknow type */
	Scierror(999, _("%s: Wrong type for input argument(s).\n"),fname);
	return 0;
}
/*--------------------------------------------------------------------------*/
static int lengthfunction(int RhsPosition,char *VariableType)
{
	int m = 0, n = 0, l = 0;
	int mn = 0;

	GetRhsVar(RhsPosition,VariableType,&m,&n,&l);
	mn = m * n;

	return mn;
}
/*--------------------------------------------------------------------------*/
static int lengthPoly(int RhsPosition)
{
      int m = 0, n = 0; /* matrix size */
      int mn = 0; /* m*n */

      int il = 0; int ilrd = 0;
      int l1 = 0;

      int lw = RhsPosition + Top - Rhs;

      l1 = *Lstk(lw);
      il = iadr(l1);

      if (*istk(il ) < 0) il = iadr(*istk(il + 1));

      /* get dimensions */
      m = getNumberOfLines(il); /* row */
      n = getNumberOfColumns(il); /* col */
      mn = m * n ;

      ilrd = il + 4;

      /* readjust stack before to call createvar */
      C2F(intersci).ntypes[RhsPosition - 1] = '$';
      C2F(intersci).iwhere[RhsPosition - 1] = l1;
      C2F(intersci).lad[RhsPosition - 1] = l1;

      return mn;
}
/*--------------------------------------------------------------------------*/
/* !!! WARNING !!! */
/* Compatibility with Scilab 4.x */
/* length returned is the max of dimensions of the sparse matrix max(m,n) */
/* and not m * n */
static int lengthSparse(int RhsPosition)
{
	int m = 0, n = 0; /* matrix size */
    int lengthreturned = 0;

    int il = 0; int ilrd = 0;
    int l1 = 0;

    int lw = RhsPosition + Top - Rhs;

    l1 = *Lstk(lw);
    il = iadr(l1);

    if (*istk(il ) < 0) il = iadr(*istk(il + 1));

    /* get dimensions */
    m = getNumberOfLines(il); /* row */
    n = getNumberOfColumns(il); /* col */

	lengthreturned = Max(m,n);

    ilrd = il + 4;

    /* readjust stack before to call createvar */
    C2F(intersci).ntypes[RhsPosition - 1] = '$';
    C2F(intersci).iwhere[RhsPosition - 1] = l1;
    C2F(intersci).lad[RhsPosition - 1] = l1;
    return lengthreturned;
}
/*--------------------------------------------------------------------------*/
