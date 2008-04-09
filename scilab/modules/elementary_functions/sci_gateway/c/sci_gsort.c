/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste SILVY
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Cong WU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_elementary_functions.h"
#include "stack-c.h"
#include "gsort.h"
#include "string.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "sortTemplate.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "isanan.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_gsort)(char *fname, unsigned long fname_len)
{
  char iord[2] ;
  char typex[10];
  SciIntMat Im;
  int Type = 0;
  char **S = NULL;
  int m1 = 0,n1 = 0,l1 = 0 ;
  int m2 = 0,n2 = 0,l2 = 0 ;
  int m3 = 0,n3 = 0,l3 = 0 ;
  int ind_m1 = 0, ind_n1 = 0, ind_l1 =0;
  int *indices = NULL;
  int iflag = 0;

  iord[0] = DECREASE_COMMAND; 
  iord[1]='\0';

  typex[0] = GLOBAL_SORT; 
  typex[1] = '\0';

  Rhs = Max(0, Rhs);
  CheckRhs(1,3);
  CheckLhs(1,2);

  if (Rhs >= 1)
    {
      Type=VarType(1);
      switch (Type)
	{
	case sci_strings :
	  GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&S);
	  break;
	case sci_matrix :
	  {
	    int i;
	    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
	    if ( (m1 * n1) == 0 ) /* [] returns []*/
	      {
		int m = 0, n = 0, l = 0;
		CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m,&n,&l);
		LhsVar(1) = Rhs+1 ;

		if (Lhs == 2)
		  {
		    CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&m,&n,&l);
		    LhsVar(2) = Rhs+2 ;
		  }

		C2F(putlhsvar)();
		return 0;
	      }
	    for (i=0; i<(m1 * n1); i++) {
	      if (C2F(isanan)(stk(l1+i)) == 1) {
		Scierror(999,_("%s: First input argument must not contain NaNs.\n"),fname);
		return 0;
	      }
	    }

	  }
	
	  break;
	case sci_ints:
	  GetRhsVar(1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&m1,&n1,&Im);
	  break;
	default :
	  Scierror(999,_("%s: Wrong type for first input argument: Scalar or matrix of strings expected.\n"),fname);
	  return 0;
	}
    }

  if (Rhs >= 2)
    {
      char c;
      GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
      if ( m2 == 0 )
	{
	  Scierror(999,_("%s: Wrong size for second input argument: Non-empty string expected.\n"),fname);
	  return 0;
	}
      c = *cstk(l2);
      if ( (c != ROW_SORT) && (c != COLUMN_SORT) && (c != GLOBAL_SORT) && (c != LIST_SORT) )
	{
	  Scierror(999,_("%s: Wrong value for second input argument: ''%s'', ''%s'', ''%s'', ''%s'' or ''%s'' expected.\n"),fname,"r","c","g","lr","lc");
	  return 0;
	}
      strcpy(typex,cstk(l2));
    }

  if (Rhs == 3)
    {
      GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);
      CheckLength(3,m3,1);
      if ( (*cstk(l3) != INCREASE_COMMAND) && (*cstk(l3) != DECREASE_COMMAND) )
	{
	  Scierror(999,_("%s: Wrong value for third input argument: ''%s'' or ''%s'' expected.\n"),fname,"i","d");
	  return 0;
	}
      iord[0] = *cstk(l3);
    }

  if ( typex[0] == LIST_SORT) 
    {
      if (typex[1] == ROW_SORT) 
	{
	  ind_m1 = m1;
	  ind_n1 = 1;
	  ind_l1 = 0;
	  if (ind_m1 != 0) indices = (int*)MALLOC(sizeof(int)*(ind_m1));   /* Only return in row*/
	}
      else 
	{
	  ind_m1 = 1;
	  ind_n1 = n1;
	  ind_l1 = 0;
	  if (ind_n1 != 0) indices = (int*)MALLOC(sizeof(int)*(ind_n1));  /*Only return in col */
	}
    }
  else 
    {
      ind_m1 = m1;
      ind_n1 = n1;
      ind_l1 = 0;
      if ( ind_m1*ind_n1 != 0 )indices = (int*)MALLOC(sizeof(int)*(ind_m1*ind_n1));  /* return a matrix*/
    }

  if (Lhs == 2) iflag = 1; 
  else iflag = 0;

  switch (Type)
    {
    case sci_matrix:
      {
	double *matrix = stk(l1);
	double *tmp_matrix = NULL;
	if ( m1*n1 != 0 ) 
	  {
	    tmp_matrix = (double*)MALLOC(sizeof(double)*(m1*n1));
	    if (tmp_matrix)
	      {
		int i = 0;
		for (i = 0;i< m1*n1; i++) tmp_matrix[i] = matrix[i];

		C2F(gsortd)(tmp_matrix,indices,&iflag,&m1,&n1,typex,iord);

		CreateVarFromPtr(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&tmp_matrix);
		LhsVar(1)= Rhs+1 ;

		if (Lhs == 2)
		  {
		    CreateVarFromPtr(Rhs+2,MATRIX_OF_INTEGER_DATATYPE,&ind_m1,&ind_n1,&indices)
		      LhsVar(2)= Rhs+2 ;
		  }
		C2F(putlhsvar)();
		if (indices) {FREE(indices); indices = NULL;}
		if (tmp_matrix)	{ FREE(tmp_matrix); tmp_matrix = NULL;}
	      }
	  }
      }
      break;

      /* Can not find a example , so can not just remove it */
    case sci_ints:               
      {
	switch(Im.it) /* Type defined in stack-c.h */
	  {
	  case I_CHAR :
	    C2F(gsortchar)(Im.D,indices,&iflag,&m1,&n1,typex,iord);
	    break;
	  case I_INT32 :
	    C2F(gsortint)(Im.D,indices,&iflag,&m1,&n1,typex,iord);
	    break;
	  case I_UCHAR :
	    C2F(gsortuchar)(Im.D,indices,&iflag,&m1,&n1,typex,iord);
	    break;
	  case I_INT16 :
	    C2F(gsortshort)(Im.D,indices,&iflag,&m1,&n1,typex,iord);
	    break;
	  case I_UINT16 :
	    C2F(gsortushort)(Im.D,indices,&iflag,&m1,&n1,typex,iord);
	    break;
	  case I_UINT32 :
	    C2F(gsortuint)(Im.D,indices,&iflag,&m1,&n1,typex,iord);
	    break;
	  default:
	    Scierror(999,_("%s: Wrong type for first input argument: Unknown type.\n"),fname);
	    return 0;
	  }

	LhsVar(1) = Rhs + 1 ;

	if (Lhs == 2)
	  {
	    CreateVarFromPtr(Rhs+2,MATRIX_OF_INTEGER_DATATYPE,&ind_m1,&ind_n1,&indices)
	      LhsVar(2)= Rhs+2 ;
	  }
	C2F(putlhsvar)();
	if (indices) {FREE(indices); indices = NULL;}
      }
      break;

    case sci_strings:
      {
	C2F(gsorts)(S,indices,&iflag,&m1,&n1,typex,iord);
	CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &m1, &n1, S);
	LhsVar(1)=Rhs+1;

	if (Lhs == 2)
	  {
	    CreateVarFromPtr(Rhs+2,MATRIX_OF_INTEGER_DATATYPE,&ind_m1,&ind_n1,&indices)
	      LhsVar(2)= Rhs+2 ;
	  }
	C2F(putlhsvar)();
	if (indices) {FREE(indices); indices = NULL;}
	freeArrayOfString(S,m1*n1);
      }
      break;

    default:
      Scierror(999,_("%s: Wrong type for first input argument.\n"),fname);
      return 0;
      break;
    }

  return 0;
} 
/*-----------------------------------------------------------------------------------*/

