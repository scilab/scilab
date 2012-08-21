/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DatatipFunctions.hxx"
#include "ScilabView.hxx"

extern "C"
{
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "Scierror.h"
#include "gw_gui.h"
#include "BOOL.h"
#include "MALLOC.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "HandleManagement.h"
#include "FigureList.h"
#include "CurrentFigure.h"
#include "BuildObjects.h"
}

using namespace org_scilab_modules_gui_datatip;

int sci_datatipcreate(char *fname, unsigned long fname_len)
{
    int n = 0, nbRow = 0, nbCol = 0, l1 = 0, i = 0;
    int* piAddr	= NULL;
    int out_index = 0;
    int stkAdr = 0;
    int indexPoint = 0;
    char* datatip_handler = NULL;
    char* pstFigureUID = NULL;
    double* pdblReal = NULL;
    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (Rhs == 2)
    {

        /* Get Polyline Handler */
        GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);

        if (nbRow * nbCol != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
            return FALSE;
        }

        pstFigureUID = (char *)getObjectFromHandle((unsigned long) * (hstk(stkAdr)));

        /* Get Second Input Argument - Index Or Coordinates*/
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &nbRow, &nbCol, &pdblReal);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if (nbRow * nbCol == 1)
        {

            /* Create Datatip Using "datatip_handle = datatipCreate(polyline_handle,index)" */

            indexPoint = (int) pdblReal[0];

            datatip_handler = DatatipCreate::createDatatipProgramIndex(getScilabJavaVM(), (char*)pstFigureUID, indexPoint);

        }
        else if (nbRow * nbCol == 2)
        {

            /* Create Datatip Using "datatip_handle = datatipCreate(polyline_handle,pt)" */

            datatip_handler = DatatipCreate::createDatatipProgramCoord(getScilabJavaVM(), (char*)pstFigureUID, pdblReal, 2);
            //char* DatatipCreate::createDatatipProgramCoord (JavaVM * jvm_, char const* polylineUid, double const* coordDoubleXY, int coordDoubleXYSize);
        }

    }
    else
    {

        Scierror(999, _("%s: Wrong number for input argument: %d expected.\n"), fname, 2);
        return FALSE;
    }

    nbRow = 1;
    nbCol = 1;
    CreateVar(Rhs + 1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &out_index);
    hstk(out_index)[0] = getHandle(datatip_handler);
    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return TRUE;
}

