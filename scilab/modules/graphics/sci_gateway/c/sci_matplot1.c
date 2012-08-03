/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - DIGITEO - Manuel Juliachs
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_matplot1.c                                                   */
/* desc : interface for matplot1 routine                                  */
/*------------------------------------------------------------------------*/
#include "stack-c.h"
#include "gw_graphics.h"
#include "BuildObjects.h"
#include "sciCall.h"

/*--------------------------------------------------------------------------*/
int sci_matplot1( char * fname, unsigned long fname_len )
{
    int m1 = 0, n1 = 0, l1 = 0,m2 = 0,n2 = 0,l2 = 0;
    if ( Rhs <= 0 )
    {
        sci_demo(fname, fname_len);
        return 0;
    }

    CheckRhs(2,2);
    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
    GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
    CheckLength(2,m2*n2,4);
    if ( m1 * n1 == 0 )
    {
        LhsVar(1) = 0;
        PutLhsVar();
        return 0;
    }

    getOrCreateDefaultSubwin();

    /* NG beg */
    Objmatplot1 (stk(l1), &m1, &n1,stk(l2));

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
} 
/*--------------------------------------------------------------------------*/
