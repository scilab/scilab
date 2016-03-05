/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_matplot1(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int m1 = 0, n1 = 0, m2 = 0, n2 = 0;
    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int type;
    int plottype = -1;
    int precision;
    int *dims = NULL;

    double* l1 = NULL;
    double* l2 = NULL;


    if (in.size() < 1)
    {
        return Overload::call(L"%_Matplot1", in, _iRetCount, out);
    }
    else if (in.size() != 2)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "Matplot1", 2);
        return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        types::Double *pDbl = in[0]->getAs<types::Double>();
        l1 = pDbl->get();
        if (pDbl->getDims() > 2)
        {
            dims = pDbl->getDimsArray();
            if (pDbl->getDims() > 3 || (dims[2] != 1 && dims[2] != 3 && dims[2] != 4))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "Matplot1", 1);
                return types::Function::Error;
            }

            m1 = dims[0];
            n1 = dims[1];
            if (dims[2] == 1)
            {
                plottype = buildMatplotType(MATPLOT_HM1_Double, MATPLOT_FORTRAN, MATPLOT_GRAY);
            }
            else if (dims[2] == 3)
            {
                plottype = buildMatplotType(MATPLOT_HM3_Double, MATPLOT_FORTRAN, MATPLOT_RGB);
            }
            else
            {
                plottype = buildMatplotType(MATPLOT_HM4_Double, MATPLOT_FORTRAN, MATPLOT_RGBA);
            }
        }
        else
        {
            m1 = pDbl->getRows();
            n1 = pDbl->getCols();
            plottype = buildMatplotType(MATPLOT_Double, MATPLOT_FORTRAN, MATPLOT_INDEX);
        }
    }
    else if (in[0]->isInt8())
    {
        types::Int8 *pDbl = in[0]->getAs<types::Int8>();
        l1 = (double *)pDbl->get();
        if (pDbl->getDims() > 2)
        {

            dims = pDbl->getDimsArray();
            if (pDbl->getDims() > 3 || (dims[2] != 1 && dims[2] != 3 && dims[2] != 4))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "Matplot1", 1);
                return types::Function::Error;
            }

            m1 = dims[0];
            n1 = dims[1];
            if (dims[2] == 1)
            {
                plottype = buildMatplotType(MATPLOT_HM1_Char, MATPLOT_FORTRAN, MATPLOT_GRAY);
            }
            else if (dims[2] == 3)
            {
                plottype = buildMatplotType(MATPLOT_HM1_Char, MATPLOT_FORTRAN, MATPLOT_RGB);
            }
            else
            {
                plottype = buildMatplotType(MATPLOT_HM1_Char, MATPLOT_FORTRAN, MATPLOT_RGBA);
            }
        }
        else
        {
            m1 = pDbl->getRows();
            n1 = pDbl->getCols();
            plottype = buildMatplotType(MATPLOT_Char, MATPLOT_FORTRAN, MATPLOT_INDEX);
        }
    }
    else if (in[0]->isUInt8())
    {
        types::UInt8 *pDbl = in[0]->getAs<types::UInt8>();
        l1 = (double *)pDbl->get();
        if (pDbl->getDims() > 2)
        {

            dims = pDbl->getDimsArray();
            if (pDbl->getDims() > 3 || (dims[2] != 1 && dims[2] != 3 && dims[2] != 4))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "Matplot1", 1);
                return types::Function::Error;
            }

            m1 = dims[0];
            n1 = dims[1];
            if (dims[2] == 1)
            {
                plottype = buildMatplotType(MATPLOT_HM1_UChar, MATPLOT_FORTRAN, MATPLOT_GRAY);
            }
            else if (dims[2] == 3)
            {
                plottype = buildMatplotType(MATPLOT_HM1_UChar, MATPLOT_FORTRAN, MATPLOT_RGB);
            }
            else
            {
                plottype = buildMatplotType(MATPLOT_HM1_UChar, MATPLOT_FORTRAN, MATPLOT_RGBA);
            }
        }
        else
        {
            m1 = pDbl->getRows();
            n1 = pDbl->getCols();
            plottype = buildMatplotType(MATPLOT_UChar, MATPLOT_FORTRAN, MATPLOT_INDEX);
        }
    }
    else if (in[0]->isInt16())
    {
        types::Int16 *pDbl = in[0]->getAs<types::Int16>();
        l1 = (double *)pDbl->get();
        m1 = pDbl->getRows();
        n1 = pDbl->getCols();
        plottype = buildMatplotType(MATPLOT_Short, MATPLOT_FORTRAN, MATPLOT_INDEX);
    }
    else if (in[0]->isUInt16())
    {
        types::UInt16 *pDbl = in[0]->getAs<types::UInt16>();
        l1 = (double *)pDbl->get();
        m1 = pDbl->getRows();
        n1 = pDbl->getCols();
        plottype = buildMatplotType(MATPLOT_UShort, MATPLOT_FORTRAN, MATPLOT_INDEX);
    }
    else if ((in[0]->isInt32()) || (in[0]->isInt64()))
    {
        types::Int32 *pDbl = in[0]->getAs<types::Int32>();
        l1 = (double *)pDbl->get();
        m1 = pDbl->getRows();
        n1 = pDbl->getCols();
        plottype = buildMatplotType(MATPLOT_Int, MATPLOT_FORTRAN, MATPLOT_INDEX);
    }
    else if ((in[0]->isUInt32()) || (in[0]->isUInt64()))
    {
        types::UInt32 *pDbl = in[0]->getAs<types::UInt32>();
        l1 = (double *)pDbl->get();
        m1 = pDbl->getRows();
        n1 = pDbl->getCols();
        plottype = buildMatplotType(MATPLOT_UInt, MATPLOT_FORTRAN, MATPLOT_INDEX);
    }

    if (!(in[1]->isDouble()))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), "Matplot1", 2);
        return types::Function::Error;

    }
    types::Double *pDbl1 = in[1]->getAs<types::Double>();
    if (pDbl1->getDims() > 2)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Row vector expected.\n"), "Matplot1", 2);
        return types::Function::Error;
    }

    l2 = pDbl1->get();
    m2 = pDbl1->getRows();
    n2 = pDbl1->getCols();

    //CheckLength
    if (m2 * n2 != 4)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %d expected.\n"), "Matplot1", 2, m2 * n2);
        return types::Function::Error;
    }

    getOrCreateDefaultSubwin();

    /* NG beg */
    Objmatplot1 ((l1), &m1, &n1, (l2), plottype);


    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
