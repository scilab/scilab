/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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
#include "os_string.h"
#include "CurrentObject.h"
#include "HandleManagement.h"
#include "Axes.h"
}

/*--------------------------------------------------------------------------*/
static void internal_cleanup(char* strf, int* nax, int* frameflag, int* axesflag);
/*--------------------------------------------------------------------------*/
void matplot_parse_input(types::typed_list &in, void **l1, int *n1, int *m1, int *plottype, const char* fname);
/*--------------------------------------------------------------------------*/
static const char* fname = "Matplot";
types::Function::ReturnValue sci_matplot(types::typed_list &in, types::optional_list &opt, int _iRetCount, types::typed_list &out)
{
    int m1 = 0;
    int n1 = 0;
    int frame_def = 8;
    int *frame = &frame_def;
    int axes_def = 1;
    int *axes = &axes_def;
    int *frameflag = NULL;
    int *axesflag  = NULL;

    char* strf      = NULL ;
    double* rect    = NULL ;
    int* nax        = NULL ;
    BOOL flagNax    = FALSE;

    void* l1 = NULL;
    int plottype = -1;

    if (in.size() < 1)
    {
        return Overload::call(L"%_Matplot", in, _iRetCount, out);
    }

    if (in.size() > 5)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), fname, 1, 5);
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

    if (in.size() > 1)
    {
        if (in[1]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
            return types::Function::Error;
        }

        strf =  wide_string_to_UTF8(in[1]->getAs<types::String>()->get(0));
        if (in.size() > 2)
        {
            if (in[2]->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 3);
                FREE(strf);
                return types::Function::Error;
            }

            rect =  in[2]->getAs<types::Double>()->get();
            if (in.size() > 3)
            {
                if (in[3]->isDouble() == false)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 4);
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
        Scierror(999, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), fname, 1, 5);
        internal_cleanup(strf, nax, frameflag, axesflag);
        return types::Function::Error;
    }

    // get optional argument if necessary
    for (const auto& o : opt)
    {
        if (o.first == L"strf")
        {
            if (o.second->isString() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%ls: string expected.\n"), fname, o.first.c_str());
                internal_cleanup(strf, nax, frameflag, axesflag);
                return types::Function::Error;
            }

            if (strf)
            {
                continue;
            }

            strf =  wide_string_to_UTF8(o.second->getAs<types::String>()->get(0));
        }
        else
        {
            if (o.second->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%ls: A matrix expected.\n"), fname, o.first.c_str());
                internal_cleanup(strf, nax, frameflag, axesflag);
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

    // Matplot only has a bounding rectangle, its x and y coordinates are always linear
    if (hasXYLogScale(getOrCreateDefaultSubwin()))
    {
        Scierror(999, _("%s: Logarithmic scale cannot be used.\n"), fname);
        internal_cleanup(strf, nax, frameflag, axesflag);
        return types::Function::Error;
    }

    if (strf == NULL)
    {
        reinitDefStrfN();

        strf = os_strdup(DEFSTRFN);

        if (!isDefRect(rect))
        {
            strf[1] = '7';
        }

        if (frameflag != &frame_def)
        {
            strf[1] = (char)(*frame + 48);
        }

        if (axesflag != &axes_def)
        {
            strf[2] = (char)(*axes + 48);
        }
    }

    // In function of the 'strf' second value, 'rec' has to be defined.
    if(rect == NULL && (strf[1] == '1' || strf[1] == '3' || strf[1] == '5' || strf[1] == '7'))
    {
        Scierror(999, _("%s: Wrong value for input argument #%d or missing '%s' argument.\n"), fname, 2, "rect");
        internal_cleanup(strf, nax, frameflag, axesflag);
        return types::Function::Error;
    }

    ObjmatplotImage(l1, &m1, &n1, strf, rect, nax, flagNax, plottype);
    internal_cleanup(strf, nax, frameflag, axesflag);

    if (_iRetCount == 1)
    {
        out.push_back(new types::GraphicHandle(getHandle(getCurrentObject())));
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
void matplot_parse_input(types::typed_list &in, void **pl1, int *pn1, int *pm1, int *pplottype, const char *fname)
{
    int *dims = NULL;
    void *l1 = NULL;
    int n1 = 0;
    int m1 = 0;
    int plottype = -1;
    
    *pn1 = 0;
    *pm1 = 0;
    
    if (in[0]->isDouble())
    {
        types::Double *pIn = in[0]->getAs<types::Double>();
        l1 = (void*) pIn->get();
        if (pIn->getDims() > 2)
        {
            dims = pIn->getDimsArray();
            if (pIn->getDims() > 3 || (dims[2] != 1 && dims[2] != 3 && dims[2] != 4))
            {
                Scierror(999, _("%s: Wrong dimensions for input argument #%d: (n,m,p) with p = 1, 3 or 4 expected.\n"), fname, 1);
                return;
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
                Scierror(999, _("%s: Wrong dimensions for input argument #%d: (n,m,p) with p = 1, 3 or 4 expected.\n"), fname, 1);
                return;
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
                Scierror(999, _("%s: Wrong dimensions for input argument #%d: (n,m,p) with p = 1, 3 or 4 expected.\n"), fname, 1);
                return;
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
        Scierror(999, _("%s: Wrong type for input argument #%d: A real or integer array expected.\n"), fname, 1);
    }
    
    *pl1 = l1;
    *pn1 = n1;
    *pm1 = m1;
    *pplottype = plottype;
}    

/*--------------------------------------------------------------------------*/
static void internal_cleanup(char* strf, int* nax, int* frameflag, int* axesflag)
{
    if (strf)
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
}
/*--------------------------------------------------------------------------*/
