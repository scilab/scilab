/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/

/*--------------------------------------------------------------------------*/
#include "gw_windows_tools.h"
#include "httpdownloadfile.h"
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_winhttpdownload(char *fname, unsigned long l)
{
    //int m1 = 0, n1 = 0, l1 = 0;
    //int m2 = 0, n2 = 0, l2 = 0;
    //int *Status = NULL;

    //CheckRhs(2, 2);
    //CheckLhs(1, 2);

    //if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
    //{
    //    int *Status = NULL;
    //    httpdownloadfile_error_code result;

    //    GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
    //    GetRhsVar(2, STRING_DATATYPE, &m2, &n2, &l2);

    //    result = httpDownloadFile(cstk(l1), cstk(l2));

    //    if (Lhs == 2)
    //    {
    //        int numRow   = 1 ;
    //        int numCol   = 1 ;
    //        int outIndex = 0 ;
    //        CreateVar( Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &numRow, &numCol, &outIndex );
    //        *stk(outIndex) = (int)result ;

    //    }

    //    Status = (int*)MALLOC(sizeof(int));

    //    if (result == 0)
    //    {
    //        *Status = TRUE;
    //    }
    //    else
    //    {
    //        *Status = FALSE;
    //    }

    //    m1 = 1;
    //    n1 = 1;
    //    CreateVarFromPtr(Rhs + 1, MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &Status);

    //    if (Lhs == 2)
    //    {
    //        LhsVar(2) = Rhs + 2;
    //    }
    //    LhsVar(1) = Rhs + 1;

    //    PutLhsVar();;
    //    if (Status)
    //    {
    //        FREE(Status);
    //        Status = NULL;
    //    }

    //}
    //else
    //{
    //    Scierror(999, _("%s: Wrong type for input arguments: Strings expected.\n"), fname);
    //}
    //return 0;
}
/*--------------------------------------------------------------------------*/
