/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "callfftw.h"
#include "gw_fftw.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
int sci_disposefftwlibrary(char *fname,unsigned long fname_len)
{
    static int l1,n1;

    n1=1;
    if ( DisposeFFTWLibrary() )
    {
        CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
        *istk(l1)=(int)(TRUE);
    }
    else
    {
        CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
        *istk(l1)=(int)(FALSE);
    }

    LhsVar(1)=Rhs+1;
    PutLhsVar();

    return(0);
}
/*--------------------------------------------------------------------------*/
