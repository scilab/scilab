/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

#ifndef __STACK_OPTIONAL__
#define __STACK_OPTIONAL__

#define CheckOpt(first) if ( C2F(checkopt)(first) ) {return 0;}

#define FirstOpt() C2F(firstopt)()

#define FindOpt(str,opts) C2F(findopt)(str,opts)

#define NumOpt() C2F(numopt)()

#define IsOpt(k,name) C2F(isopt)((c_local=k,&c_local),name,nlgh)

/*-------------------------------------------------
 * structure used for optional arguments in interfaces
 * -------------------------------------------------*/
typedef struct rhs_opts__
{
    int iPos ; /** stack position : -1 if not present */
    char* pstName;
    int iType;// -1 not yet defined
    int iRows;
    int iCols;
    int* piAddr;
} rhs_opts;

#endif /* __STACK_OPTIONAL__ */
