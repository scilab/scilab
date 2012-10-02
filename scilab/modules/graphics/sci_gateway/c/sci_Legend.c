/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_Legend.c                                                     */
/* desc : interface for captions routine                                  */
/*------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>

#include "gw_graphics.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "DrawObjects.h"
#include "Axes.h" /* propertyNameToLegendPlace */
#include "HandleManagement.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentObject.h"

#define DEF_LEGEND_LOCATION "in_upper_right"

/*--------------------------------------------------------------------------*/
int sci_Legend( char * fname, unsigned long fname_len )
{
    int numrow = 0, numcol = 0, l1 = 0, l2 = 0, n = 0, m2 = 0, n2 = 0;
    long handlesvalue = 0;
    int outindex = 0, i = 0;
    char *pobjUID = NULL;
    long long *tabofhandles = NULL;
    char * psubwinUID = NULL;
    char * pFigureUID = NULL;
    sciLegendPlace location;
    int type = -1;
    int *piType = &type;
    char **Str = NULL;
    char * legendUID = NULL;

    CheckRhs(2, 3);
    CheckLhs(0, 1);


    GetMatrixdims(1, &numrow, &numcol);
    n = numrow * numcol;
    if (numrow == 0 || numcol == 0)
    {
        CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &numrow, &numcol, &l1);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();
        return 0;
    }
    GetMatrixdims(2, &m2, &n2);
    if (m2*n2 != n)
    {
        Scierror(999, _("%s: Wrong size for input arguments #%d and #%d: Incompatible length.\n"), fname, 1, 2);
        return 0;
    }


    GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &numrow, &numcol, &l1);
    GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &m2, &n2, &Str);
    if (Rhs == 3)
    {
        GetRhsVar(3, STRING_DATATYPE, &m2, &n2, &l2);
        location = propertyNameToLegendPlace(cstk(l2));

        if (location == SCI_LEGEND_POSITION_UNSPECIFIED)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Incorrect value.\n"), fname, 3);
            return 0;
        }
    }
    else
    {
        location = propertyNameToLegendPlace(DEF_LEGEND_LOCATION);
    }

    tabofhandles = (long long *)MALLOC(n * sizeof(long long));
    if (tabofhandles == NULL)
    {
        freeArrayOfString(Str, n);
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    for (i = 0; i < n; i++)
    {
        char* subwinUID;

        handlesvalue = (unsigned long) (hstk(l1))[n - 1 - i];
        pobjUID = (char*)getObjectFromHandle(handlesvalue);

        if (pobjUID == NULL)
        {
            freeArrayOfString(Str, n);
            FREE(tabofhandles);
            Scierror(999, _("%s: The handle is no more valid.\n"), fname);
            return 0;
        }

       /**
         * We get the current pSubwin & pFigure from the first handle's parents.
         */
        if (i == 0)
        {
            getGraphicObjectProperty(pobjUID, __GO_PARENT_FIGURE__, jni_string, (void **)&pFigureUID);
            getGraphicObjectProperty(pobjUID, __GO_PARENT_AXES__, jni_string, (void **)&psubwinUID);
        }

        /**
         * We check that the pSubwin UID is the same for all given handles.
         */
        getGraphicObjectProperty(pobjUID, __GO_PARENT_AXES__, jni_string, (void **)&subwinUID);

        if (strcmp(psubwinUID, subwinUID) != 0)
        {
            Scierror(999, _("%s: Objects must have the same axes.\n"), fname);
            FREE(tabofhandles);
            return 0;
        }

        getGraphicObjectProperty(pobjUID, __GO_TYPE__, jni_int, (void **)&piType);

        if (type != __GO_POLYLINE__)
        {
            freeArrayOfString(Str, n);
            FREE(tabofhandles);
            Scierror(999, _("%s: The %d th handle is not a polyline handle.\n"), fname, i + 1);
            return 0;
        }

        tabofhandles[i] = handlesvalue;
    }

    /* Create the legend */
    legendUID = ConstructLegend (psubwinUID, Str, tabofhandles, n);

    setGraphicObjectProperty(legendUID, __GO_LEGEND_LOCATION__, &location, jni_int, 1);

    freeArrayOfString(Str, n);
    FREE(tabofhandles);

    /* Return the handle of the newly created legend */
    numrow = 1;
    numcol = 1;
    CreateVar(Rhs + 1, GRAPHICAL_HANDLE_DATATYPE, &numrow, &numcol, &outindex);
    hstk(outindex)[0] = getHandle((char *) getCurrentObject());
    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
