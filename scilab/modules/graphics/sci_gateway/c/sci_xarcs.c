/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xarcs.h                                                       */
/* desc : interface for xarcs routine                                      */
/*------------------------------------------------------------------------*/
#include "stack-c.h"
#include "gw_graphics.h"
#include "BuildObjects.h"
#include "sciCall.h"
#include "localization.h"
#include "Scierror.h"

#include "CurrentObject.h"

#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"

/*--------------------------------------------------------------------------*/
int sci_xarcs(char *fname, unsigned long fname_len)
{
    int m1 = 0, n1 = 0, l1 = 0, m2 = 0, n2 = 0, l2 = 0;

    long hdl = 0;
    int i = 0, i2 = 0;
    double angle1 = 0.0;
    double angle2 = 0.0;

    char *pstCurrentSubWinUID = NULL;
    int iCurrentSubWinForeground = 0;
    int *piCurrentSubWinForeground = &iCurrentSubWinForeground;

    CheckRhs(1, 2);

    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
    if (strcmp(fname, "xarcs") == 0)
    {
        if (m1 != 6)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: %s expected.\n"), fname, 1, "(6,n)");
            return 0;
        }
    }
    else
    {
        if (m1 != 4)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: %s expected.\n"), fname, 1, "(4,n)");
            return 0;
        }
    }

    pstCurrentSubWinUID = (char*)getOrCreateDefaultSubwin();

    if (Rhs == 2)
    {
        GetRhsVar(2, MATRIX_OF_INTEGER_DATATYPE, &m2, &n2, &l2);
        CheckVector(2, m2, n2);
        if (m2 * n2 != n1)
        {
            Scierror(999, _("%s: Wrong size for arguments #%d and #%d.\n"), fname, 1, 2);
            return 0;
        }
    }
    else
    {
        m2 = 1;
        n2 = n1;
        CreateVar(2, MATRIX_OF_INTEGER_DATATYPE, &m2, &n2, &l2);
        getGraphicObjectProperty(pstCurrentSubWinUID, __GO_LINE_COLOR__, jni_int, (void **)&piCurrentSubWinForeground);
        for (i2 = 0; i2 < n2; ++i2)
        {
            *istk(l2 + i2) = iCurrentSubWinForeground;
        }
    }

    for (i = 0; i < n1; ++i)
    {
        angle1 = DEG2RAD(*stk(l1 + (6 * i) + 4) / 64.0);
        angle2 = DEG2RAD(*stk(l1 + (6 * i) + 5) / 64.0);
        Objarc(&angle1, &angle2, stk(l1 + (6 * i)), stk(l1 + (6 * i) + 1),
               stk(l1 + (6 * i) + 2), stk(l1 + (6 * i) + 3), istk(l2 + i), NULL, FALSE, TRUE, &hdl);
    }

    /* construct Compound and make it current object */
    {
        char * o = ConstructCompoundSeq(n1);
        setCurrentObject(o);
        releaseGraphicObjectProperty(__GO_PARENT__, o, jni_string, 1);
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}

/*--------------------------------------------------------------------------*/
