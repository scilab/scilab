/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_matplot.h                                                    */
/* desc : interface for matplot routine                                   */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "GetCommandArg.h"
#include "DefaultCommandArg.h"
#include "BuildObjects.h"
#include "sciCall.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "Matplot.h"
/*--------------------------------------------------------------------------*/

int sci_matplot(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int m1 = 0, n1 = 0;
    int frame_def = 8;
    int *frame = &frame_def;
    int axes_def = 1;
    int *axes = &axes_def;
    static rhs_opts opts[] =
    {
        { -1, "axesflag", -1, 0, 0, NULL},
        { -1, "frameflag", -1, 0, 0, NULL},
        { -1, "nax", -1, 0, 0, NULL},
        { -1, "rect", -1, 0, 0, NULL},
        { -1, "strf", -1, 0, 0, NULL},
        { -1, NULL, -1, 0, 0, NULL}
    };

    char   * strf    = NULL ;
    char strfl[4];
    double* rect    = NULL ;
    int    * nax     = NULL ;
    BOOL     flagNax = FALSE;

    int* piAddr1 = NULL;
    void * l1 = NULL;
    int type = 0;
    int precision = 0;
    int plottype = -1;

    if (nbInputArgument(pvApiCtx) <= 0)
    {
        sci_demo(fname, fname_len);
        return 0;
    }
    CheckInputArgument(pvApiCtx, 1, 5);

    if (getOptionals(pvApiCtx, fname, opts) == 0)
    {
        ReturnArguments(pvApiCtx);
        return 0;
    }
    if (FirstOpt() < 2)
    {
        Scierror(999, _("%s: Misplaced optional argument: #%d must be at position %d.\n"),
                 fname, 1, 2);
        return(0);
    }

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getVarType(pvApiCtx, piAddr1, &type);
    if (sciErr.iErr)
    {
        Scierror(999, _("%s: Cannot get the data type.\n"), fname, 1);
        printError(&sciErr, 0);
        return 1;
    }

    switch (type)
    {
        case sci_matrix :
            sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &m1, &n1, (double **)&l1);
            if (sciErr.iErr)
            {
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
                printError(&sciErr, 0);
                return 1;
            }

            if (m1 * n1 == 0)
            {
                AssignOutputVariable(pvApiCtx, 1) = 0;
                ReturnArguments(pvApiCtx);
                return 0;
            }

            plottype = buildMatplotType(MATPLOT_Double, MATPLOT_FORTRAN, MATPLOT_INDEX);
            break;
        case sci_ints :
            sciErr = getMatrixOfIntegerPrecision(pvApiCtx, piAddr1, &precision);
            if (sciErr.iErr)
            {
                Scierror(999, _("%s: Cannot get the integer type for argument #%d.\n"), fname, 1);
                printError(&sciErr, 0);
                return 1;
            }

            switch (precision)
            {
                case SCI_INT8 :
                    sciErr = getMatrixOfInteger8(pvApiCtx, piAddr1, &m1, &n1, (char **)(&l1));
                    if (sciErr.iErr)
                    {
                        Scierror(202, _("%s: Cannot get the data for argument #%d.\n"), fname, 1);
                        printError(&sciErr, 0);
                        return 1;
                    }
                    plottype = buildMatplotType(MATPLOT_Char, MATPLOT_FORTRAN, MATPLOT_RGB_332);
                    break;
                case SCI_UINT8 :
                    sciErr = getMatrixOfUnsignedInteger8(pvApiCtx, piAddr1, &m1, &n1, (unsigned char **)(&l1));
                    if (sciErr.iErr)
                    {
                        Scierror(202, _("%s: Cannot get the data for argument #%d.\n"), fname, 1);
                        printError(&sciErr, 0);
                        return 1;
                    }
                    plottype = buildMatplotType(MATPLOT_UChar, MATPLOT_FORTRAN, MATPLOT_GRAY);
                    break;
                case SCI_INT16 :
                    sciErr = getMatrixOfInteger16(pvApiCtx, piAddr1, &m1, &n1, (short **)(&l1));
                    if (sciErr.iErr)
                    {
                        Scierror(202, _("%s: Cannot get the data for argument #%d.\n"), fname, 1);
                        printError(&sciErr, 0);
                        return 1;
                    }
                    plottype = buildMatplotType(MATPLOT_Short, MATPLOT_FORTRAN, MATPLOT_RGB_444);
                    break;
                case SCI_UINT16 :
                    sciErr = getMatrixOfUnsignedInteger16(pvApiCtx, piAddr1, &m1, &n1, (unsigned short **)(&l1));
                    if (sciErr.iErr)
                    {
                        Scierror(202, _("%s: Cannot get the data for argument #%d.\n"), fname, 1);
                        printError(&sciErr, 0);
                        return 1;
                    }
                    plottype = buildMatplotType(MATPLOT_UShort, MATPLOT_FORTRAN, MATPLOT_RGBA_4444);
                    break;
                case SCI_INT32 :
                    sciErr = getMatrixOfInteger32(pvApiCtx, piAddr1, &m1, &n1, (int **)(&l1));
                    if (sciErr.iErr)
                    {
                        Scierror(202, _("%s: Cannot get the data for argument #%d.\n"), fname, 1);
                        printError(&sciErr, 0);
                        return 1;
                    }
                    plottype = buildMatplotType(MATPLOT_Int, MATPLOT_FORTRAN, MATPLOT_RGB);
                    break;
                case SCI_UINT32 :
                    sciErr = getMatrixOfUnsignedInteger32(pvApiCtx, piAddr1, &m1, &n1, (unsigned int **)(&l1));
                    if (sciErr.iErr)
                    {
                        Scierror(202, _("%s: Cannot get the data for argument #%d.\n"), fname, 1);
                        printError(&sciErr, 0);
                        return 1;
                    }
                    plottype = buildMatplotType(MATPLOT_UInt, MATPLOT_FORTRAN, MATPLOT_RGBA);
                    break;
                default :
                    Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), fname, 1);
                    return 1;
            }
            break;
        case sci_mlist :
            if (isHypermatType(pvApiCtx, piAddr1))
            {
                int htype = 0;
                int ndims = 0;
                int * dims = NULL;

                sciErr = getHypermatType(pvApiCtx, piAddr1, &htype);
                if (sciErr.iErr)
                {
                    Scierror(202, _("%s: Cannot get the hypermatrix data type for argument #%d.\n"), fname, 1);
                    return 1;
                }

                if (htype == sci_ints)
                {
                    sciErr = getHypermatOfIntegerPrecision(pvApiCtx, piAddr1, &precision);
                    if (sciErr.iErr)
                    {
                        Scierror(202, _("%s: Cannot get the hypermatrix data type for argument #%d.\n"), fname, 1);
                        return 1;
                    }

                    if (precision != SCI_UINT8 && precision != SCI_INT8)
                    {
                        Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), fname, 1);
                        return 1;
                    }

                    if (precision == SCI_UINT8)
                    {
                        sciErr = getHypermatOfUnsignedInteger8(pvApiCtx, piAddr1, &dims, &ndims, (unsigned char **)&l1);
                    }
                    else
                    {
                        sciErr = getHypermatOfInteger8(pvApiCtx, piAddr1, &dims, &ndims, (char **)&l1);
                    }

                    if (sciErr.iErr || ndims != 3 || (dims[2] != 1 && dims[2] != 3 && dims[2] != 4))
                    {
                        Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), fname, 1);
                        return 1;
                    }

                    m1 = dims[0];
                    n1 = dims[1];
                    if (dims[2] == 1)
                    {
                        if (precision == SCI_INT8)
                        {
                            plottype = buildMatplotType(MATPLOT_HM1_Char, MATPLOT_FORTRAN, MATPLOT_GRAY);
                        }
                        else
                        {
                            plottype = buildMatplotType(MATPLOT_HM1_UChar, MATPLOT_FORTRAN, MATPLOT_GRAY);
                        }
                    }
                    else if (dims[2] == 3)
                    {
                        if (precision == SCI_INT8)
                        {
                            plottype = buildMatplotType(MATPLOT_HM3_Char, MATPLOT_FORTRAN, MATPLOT_RGB);
                        }
                        else
                        {
                            plottype = buildMatplotType(MATPLOT_HM3_UChar, MATPLOT_FORTRAN, MATPLOT_RGB);
                        }
                    }
                    else
                    {
                        if (precision == SCI_INT8)
                        {
                            plottype = buildMatplotType(MATPLOT_HM4_Char, MATPLOT_FORTRAN, MATPLOT_RGBA);
                        }
                        else
                        {
                            plottype = buildMatplotType(MATPLOT_HM4_UChar, MATPLOT_FORTRAN, MATPLOT_RGBA);
                        }
                    }
                }
                else if (htype == sci_matrix)
                {
                    sciErr = getHypermatOfDouble(pvApiCtx, piAddr1, &dims, &ndims, (double **)&l1);
                    if (sciErr.iErr || ndims != 3 || (dims[2] != 1 && dims[2] != 3 && dims[2] != 4))
                    {
                        Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), fname, 1);
                        return 1;
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
                    Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "data", 1);
                    return 1;
                }
            }
            break;
        default :
            Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), fname, 1);
            return 1;
    }

    GetStrf(pvApiCtx, fname, 2, opts, &strf);
    GetRect(pvApiCtx, fname, 3, opts, &rect);
    GetNax(pvApiCtx, 4, opts, &nax, &flagNax);

    getOrCreateDefaultSubwin();

    if (isDefStrf(strf))
    {
        strcpy(strfl, DEFSTRFN);

        strf = strfl;
        if (!isDefRect(rect))
        {
            strfl[1] = '7';
        }

        GetOptionalIntArg(pvApiCtx, fname, 5, "frameflag", &frame, 1, opts);
        if (frame != &frame_def)
        {
            strfl[1] = (char)(*frame + 48);
        }
        GetOptionalIntArg(pvApiCtx, fname, 5, "axesflag", &axes, 1, opts);
        if (axes != &axes_def)
        {
            strfl[2] = (char)(*axes + 48);
        }
    }

    ObjmatplotImage((unsigned char *)(l1), &m1, &n1, strf, rect, nax, flagNax, plottype);

    /* NG end */
    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
