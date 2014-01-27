/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_matplot1.c                                                   */
/* desc : interface for matplot1 routine                                  */
/*------------------------------------------------------------------------*/
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "gw_graphics.h"
#include "BuildObjects.h"
#include "sciCall.h"
#include "Matplot.h"

/*--------------------------------------------------------------------------*/
int sci_matplot1(char * fname, unsigned long fname_len)
{
    SciErr sciErr;
    int m1 = 0, n1 = 0, m2 = 0, n2 = 0;
    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int type;
    int plottype = -1;
    int precision;

    double* l1 = NULL;
    double* l2 = NULL;

    if (nbInputArgument(pvApiCtx) <= 0)
    {
        sci_demo(fname, fname_len);
        return 0;
    }

    CheckInputArgument(pvApiCtx, 2, 2);
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
                        Scierror(202, _("%s: Wrong type for argument #%d: A real or integer expected.\n"), fname, 1);
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

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 2.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &m2, &n2, &l2);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 2);
        printError(&sciErr, 0);
        return 1;
    }

    //CheckLength
    if (m2 * n2 != 4)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %d expected.\n"), fname, 2, m2 * n2);
        return 1;
    }

    if (m1 * n1 == 0)
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    getOrCreateDefaultSubwin();

    /* NG beg */
    Objmatplot1 ((l1), &m1, &n1, (l2), plottype);

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
