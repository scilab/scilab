/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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

/*------------------------------------------------------------------------*/
/* file: sci_matplot1.c                                                   */
/* desc : interface for matplot1 routine                                  */
/*------------------------------------------------------------------------*/

#include "graphics_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "graphichandle.hxx"
#include "overload.hxx"
#include "int.hxx"

extern "C"
{
#include <string.h>
#include "gw_graphics.h"
#include "GetCommandArg.h"
#include "DefaultCommandArg.h"
#include "BuildObjects.h"
#include "sciCall.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "Matplot.h"
#include "CurrentObject.h"
#include "HandleManagement.h"
#include "Axes.h"
}

/*--------------------------------------------------------------------------*/
void matplot_parse_input(types::typed_list &in, void **l1, int *n1, int *m1, int *plottype, const char* fname);
/*--------------------------------------------------------------------------*/
static const char* fname = "Matplot1";
types::Function::ReturnValue sci_matplot1(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int m1 = 0, n1 = 0, m2 = 0, n2 = 0;
    int plottype = -1;

    void* l1 = NULL;
    double* l2 = NULL;


    if (in.size() < 1)
    {
        return Overload::call(L"%_Matplot1", in, _iRetCount, out);
    }
    else if (in.size() != 2)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), fname, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output arguments: At most %d expected.\n"), fname, 1);
        return types::Function::Error;
    }

    matplot_parse_input(in, &l1, &n1, &m1, &plottype, fname);

    if (n1*m1 == 0)
    {
        return types::Function::Error;
    }

    if (!(in[1]->isDouble()))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 2);
        return types::Function::Error;

    }
    types::Double *pDbl1 = in[1]->getAs<types::Double>();
    if (pDbl1->getDims() > 2)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Row vector expected.\n"), fname, 2);
        return types::Function::Error;
    }

    l2 = pDbl1->get();
    m2 = pDbl1->getRows();
    n2 = pDbl1->getCols();

    //CheckLength
    if (m2 * n2 != 4)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %d expected.\n"), fname, 2, m2 * n2);
        return types::Function::Error;
    }

    // Matplot1 only has a bounding rectangle, its x and y coordinates are always linear
    if (hasXYLogScale(getOrCreateDefaultSubwin()))
    {
        Scierror(999, _("%s: Logarithmic scale cannot be used.\n"), fname);
        return types::Function::Error;
    }

    /* NG beg */
    Objmatplot1(l1, &m1, &n1, l2, plottype);

    if (_iRetCount == 1)
    {
        out.push_back(new types::GraphicHandle(getHandle(getCurrentObject())));
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
