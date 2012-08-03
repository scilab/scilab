/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_glue.h                                                       */
/* desc : interface for glue routine                                      */
/*------------------------------------------------------------------------*/

#include <string.h>
#include <stdlib.h>

#include "gw_graphics.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "MALLOC.h"
#include "GetProperty.h"
#include "elementary_functions.h"
#include "localization.h"
#include "Scierror.h"
#include "HandleManagement.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentObject.h"
/*--------------------------------------------------------------------------*/
int sci_glue( char * fname, unsigned long fname_len )
{
    int numrow = 0, numcol = 0, l1 = 0, l2 = 0, lind = 0, n = 0, cx1 = 1;
    long *handelsvalue = NULL ;
    int outindex = 0, i = 0;

    char *pstCompoundUID = NULL;
    char *pstParentUID = NULL;
    char *pstCurrentParentUID = NULL;
    char *pobjUID = NULL;

    CheckRhs(1, 1);
    CheckLhs(0, 1);

    /*  set or create a graphic window */
    GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &numrow, &numcol, &l1); /* We get the scalar value if it is ones */
    n = numrow * numcol;
    CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &numrow, &numcol, &l2);
    CreateVar(Rhs + 2, MATRIX_OF_INTEGER_DATATYPE, &numrow, &numcol, &lind);

    if (n > 1)
    {
        C2F(dcopy)(&n, stk(l1), &cx1, stk(l2), &cx1);
        C2F(dsort)(stk(l2), &n, istk(lind));
        for (i = 1; i < n; i++)
        {
            long long i1 = (long long) * hstk(l2 + i);
            long long i2 = (long long) * hstk(l2 + i - 1);

            if (i1 == i2)
            {
                Scierror(999, _("%s: Each handle should not appear twice.\n"), fname);
                return 0;
            }
        }
    }

    /* we must change the pobj to the Compound type */
    handelsvalue = MALLOC(n * sizeof(long));
    for (i = 0 ; i < n ; i++)
    {
        handelsvalue[i] = (unsigned long) (hstk(l1))[i];
        pobjUID = (char*)getObjectFromHandle(handelsvalue[i]);
        if (pobjUID == NULL)
        {
            FREE(handelsvalue);
            Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
            return 0;
        }

        getGraphicObjectProperty(pobjUID, __GO_PARENT__, jni_string, (void **)&pstCurrentParentUID);
        if (i == 0)
        {
            pstParentUID = pstCurrentParentUID;
        }

        if  (strcmp(pstParentUID, pstCurrentParentUID) != 0)
        {
            FREE(handelsvalue);
            Scierror(999, _("%s: Objects must have the same parent.\n"), fname);
            return 0;
        }

    }

    //ret = CheckForCompound (handelsvalue, n);
    //if (ret>0)
    //{
    // MEM LEAK
    //    Scierror(999,_("%s: Handle %d cannot be glued (invalid parent).\n"),fname,ret);
    //    return 0;
    //}

    //if (ret<0)
    //{
    // MEM LEAK
    //   Scierror(999,_("%s: Handle %d cannot be glued (invalid type).\n"),fname,-ret);
    //    return 0;
    //}

    pstCompoundUID = ConstructCompound(handelsvalue, n);
    setCurrentObject(pstCompoundUID);

    numrow = 1;
    numcol = 1;
    CreateVar(Rhs + 3, GRAPHICAL_HANDLE_DATATYPE, &numrow, &numcol, &outindex);
    hstk(outindex)[0] = getHandle(pstCompoundUID);
    releaseGraphicObjectProperty(__GO_PARENT__, pstCompoundUID, jni_string, 1);
    LhsVar(1) = Rhs + 3;
    PutLhsVar();
    FREE(handelsvalue);

    return 0;
}
/*--------------------------------------------------------------------------*/
