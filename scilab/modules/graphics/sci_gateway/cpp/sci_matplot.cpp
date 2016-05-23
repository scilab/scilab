/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
/* file: sci_matplot.h                                                    */
/* desc : interface for matplot routine                                   */
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
types::Function::ReturnValue sci_matplot(types::typed_list &in, types::optional_list &opt, int _iRetCount, types::typed_list &out)
{
    int m1 = 0;
    int n1 = 0;
    int *dims = NULL;
    int frame_def = 8;
    int *frame = &frame_def;
    int axes_def = 1;
    int *axes = &axes_def;
    int *frameflag = NULL;
    int *axesflag  = NULL;

    char* strf      = NULL ;
    char strfl[4];
    double* rect    = NULL ;
    int* nax        = NULL ;
    BOOL flagNax    = FALSE;

    void* l1 = NULL;
    int plottype = -1;

    bool bFREE = false;

    if (in.size() < 1)
    {
        return Overload::call(L"%_Matplot", in, _iRetCount, out);
    }

    if (in.size() > 5)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "Matplot", 1, 5);
        return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        types::Double *pIn = in[0]->getAs<types::Double>();
        l1 = (void*) pIn->get();
        if (pIn->getDims() > 2)
        {
            dims = pIn->getDimsArray();
            if (pIn->getDims() > 3 || (dims[2] != 1 && dims[2] != 3 && dims[2] != 4))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "Matplot", 1);
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
            m1 = pIn->getRows();
            n1 = pIn->getCols();
            plottype = buildMatplotType(MATPLOT_Double, MATPLOT_FORTRAN, MATPLOT_INDEX);
        }
    }
    else if (in[0]->isInt8())
    {
        types::Int8 *pIn = in[0]->getAs<types::Int8>();
        l1 = (void*) pIn->get();
        if (pIn->getDims() > 2)
        {
            dims = pIn->getDimsArray();
            if (pIn->getDims() > 3 || (dims[2] != 1 && dims[2] != 3 && dims[2] != 4))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "Matplot", 1);
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
                plottype = buildMatplotType(MATPLOT_HM3_Char, MATPLOT_FORTRAN, MATPLOT_RGB);
            }
            else
            {
                plottype = buildMatplotType(MATPLOT_HM4_Char, MATPLOT_FORTRAN, MATPLOT_RGBA);
            }
        }
        else
        {
            m1 = pIn->getRows();
            n1 = pIn->getCols();
            plottype = buildMatplotType(MATPLOT_Char, MATPLOT_FORTRAN, MATPLOT_RGB_332);
        }
    }
    else if (in[0]->isUInt8())
    {
        types::UInt8 *pIn = in[0]->getAs<types::UInt8>();
        l1 = (void*) pIn->get();
        if (pIn->getDims() > 2)
        {
            dims = pIn->getDimsArray();
            if (pIn->getDims() > 3 || (dims[2] != 1 && dims[2] != 3 && dims[2] != 4))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "Matplot", 1);
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
                plottype = buildMatplotType(MATPLOT_HM3_UChar, MATPLOT_FORTRAN, MATPLOT_RGB);
            }
            else
            {
                plottype = buildMatplotType(MATPLOT_HM4_UChar, MATPLOT_FORTRAN, MATPLOT_RGBA);
            }
        }
        else
        {
            m1 = pIn->getRows();
            n1 = pIn->getCols();
            plottype = buildMatplotType(MATPLOT_UChar, MATPLOT_FORTRAN, MATPLOT_GRAY);
        }
    }
    else if (in[0]->isInt16())
    {
        types::Int16 *pIn = in[0]->getAs<types::Int16>();
        l1 = (void*) pIn->get();
        m1 = pIn->getRows();
        n1 = pIn->getCols();
        plottype = buildMatplotType(MATPLOT_Short, MATPLOT_FORTRAN, MATPLOT_RGB_444);
    }
    else if (in[0]->isUInt16())
    {
        types::UInt16 *pIn = in[0]->getAs<types::UInt16>();
        l1 = (void*) pIn->get();
        m1 = pIn->getRows();
        n1 = pIn->getCols();
        plottype = buildMatplotType(MATPLOT_UShort, MATPLOT_FORTRAN, MATPLOT_RGBA_4444);
    }
    else if ((in[0]->isInt32()) || (in[0]->isInt64()))
    {
        types::Int32 *pIn = in[0]->getAs<types::Int32>();
        l1 = (void*) pIn->get();
        m1 = pIn->getRows();
        n1 = pIn->getCols();
        plottype = buildMatplotType(MATPLOT_Int, MATPLOT_FORTRAN, MATPLOT_RGB);
    }
    else if ((in[0]->isUInt32()) || (in[0]->isUInt64()))
    {
        types::UInt32 *pIn = in[0]->getAs<types::UInt32>();
        l1 = (void*) pIn->get();
        m1 = pIn->getRows();
        n1 = pIn->getCols();
        plottype = buildMatplotType(MATPLOT_UInt, MATPLOT_FORTRAN, MATPLOT_RGBA);
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "Matplot", 1);
        return types::Function::Error;
    }

    if (in.size() > 1)
    {
        if (in[1]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "Matplot1", 2);
            return types::Function::Error;
        }

        strf =  wide_string_to_UTF8(in[1]->getAs<types::String>()->get(0));
        bFREE = true;
        if (in.size() > 2)
        {
            if (in[2]->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), "Matplot1", 3);
                FREE(strf);
                return types::Function::Error;
            }

            rect =  in[2]->getAs<types::Double>()->get();
            if (in.size() > 3)
            {
                if (in[3]->isDouble() == false)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), "Matplot1", 4);
                    FREE(strf);
                    return types::Function::Error;
                }

                types::Double* pDbl = in[3]->getAs<types::Double>();
                double* pdbl = pDbl->get();
                int iSize = pDbl->getSize();
                nax = new int[iSize];
                for (int i = 0; i < iSize; i++)
                {
                    nax[i] = (int)pdbl[i];
                }

                flagNax = TRUE;
            }
        }
    }

    if (opt.size() > 4)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "Matplot", 1, 5);
        if (bFREE)
        {
            FREE(strf);
        }
        delete[] nax;
        return types::Function::Error;
    }

    // get optional argument if necessary
    for (const auto& o : opt)
    {
        if (o.first == L"strf")
        {
            if (o.second->isString() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%ls: string expected.\n"), "Matplot1", o.first.c_str());
                if (bFREE)
                {
                    FREE(strf);
                }

                if (nax)
                {
                    delete[] nax;
                }

                if (frameflag)
                {
                    delete[] frameflag;
                }

                if (axesflag)
                {
                    delete[] axesflag;
                }
                return types::Function::Error;
            }

            if (strf)
            {
                continue;
            }

            strf =  wide_string_to_UTF8(o.second->getAs<types::String>()->get(0));
            bFREE = true;
        }
        else
        {
            if (o.second->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%ls: A matrix expected.\n"), "Matplot1", o.first.c_str());
                if (bFREE)
                {
                    FREE(strf);
                }

                if (nax)
                {
                    delete[] nax;
                }

                if (frameflag)
                {
                    delete[] frameflag;
                }

                if (axesflag)
                {
                    delete[] axesflag;
                }
                return types::Function::Error;
            }

            types::Double* pDbl = o.second->getAs<types::Double>();
            double* pdbl = pDbl->get();
            int iSize = pDbl->getSize();

            if (o.first == L"rect" && rect == NULL)
            {
                rect = pdbl;
            }
            else if (o.first == L"nax" && nax == NULL)
            {
                nax = new int[iSize];
                for (int i = 0; i < iSize; i++)
                {
                    nax[i] = (int)pdbl[i];
                }
                flagNax = TRUE;
            }
            else if (o.first == L"frameflag" && frameflag == NULL)
            {
                frameflag = new int[iSize];
                for (int i = 0; i < iSize; i++)
                {
                    frameflag[i] = (int)pdbl[i];
                }
            }
            else if (o.first == L"axesflag" && axesflag == NULL)
            {
                axesflag = new int[iSize];
                for (int i = 0; i < iSize; i++)
                {
                    axesflag[i] = (int)pdbl[i];
                }
            }
        }
    }

    getOrCreateDefaultSubwin();

    if (strf == NULL)
    {
        reinitDefStrfN();

        strcpy(strfl, DEFSTRFN);
        strf = strfl;

        if (!isDefRect(rect))
        {
            strfl[1] = '7';
        }

        if (frameflag != &frame_def)
        {
            strfl[1] = (char)(*frame + 48);
        }

        if (axesflag != &axes_def)
        {
            strfl[2] = (char)(*axes + 48);
        }
    }

    ObjmatplotImage(l1, &m1, &n1, strf, rect, nax, flagNax, plottype);

    if (bFREE)
    {
        FREE(strf);
    }

    if (nax)
    {
        delete[] nax;
    }

    if (frameflag)
    {
        delete[] frameflag;
    }

    if (axesflag)
    {
        delete[] axesflag;
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
