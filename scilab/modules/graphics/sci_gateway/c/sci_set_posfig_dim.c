/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_set_posfig_dim.c                                             */
/* desc : interface for set_posfig_dim routine                            */
/*------------------------------------------------------------------------*/

#include "sci_set_posfig_dim.h"
#include "stack-c.h"


/*--------------------------------------------------------------------------*/
int sci_set_posfig_dim( char * fname, unsigned long fname_len )
{
  int m1,n1,l1,m2,n2,l2;
  CheckRhs(2,2);
  CheckLhs(1,1);
  GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
  GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
  /* TODO */
  LhsVar(1)=0;
  return 0;
}
/*--------------------------------------------------------------------------*/
