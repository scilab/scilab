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
/* file: sci_drawlater.c                                                  */
/* desc : interface for drawlater routine                                 */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
/*--------------------------------------------------------------------------*/
int sci_drawlater( char * fname, unsigned long fname_len )
{
    int iFalse =  (int)FALSE;
    char* pFigureUID = NULL;
    char* pSubwinUID = NULL;

    CheckRhs(0, 0);
    CheckLhs(0, 1);

    if (Rhs <= 0)
    {
        pSubwinUID = (char*)getOrCreateDefaultSubwin();
        if (pSubwinUID != NULL)
        {
            getGraphicObjectProperty(pSubwinUID, __GO_PARENT__, jni_string, (void **)&pFigureUID);
            if (pFigureUID != NULL)
            {
                setGraphicObjectProperty(pFigureUID, __GO_IMMEDIATE_DRAWING__, &iFalse, jni_bool, 1);
            }
        }
    }

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
