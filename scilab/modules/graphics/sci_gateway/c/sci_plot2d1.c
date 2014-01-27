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
/* file: sci_plot2d1.c                                                    */
/* desc : interface for plot2d1, plot2d2, plot2d3 and plot2d4 routines    */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "api_scilab.h"
#include "GetCommandArg.h"
#include "DefaultCommandArg.h"
#include "sciCall.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"

/*--------------------------------------------------------------------------*/
int sci_plot2d1_1 (char *fname, unsigned long fname_len)
{
    return sci_plot2d1_G("plot2d1", 1, fname_len); /* NG */
}
/*--------------------------------------------------------------------------*/
int sci_plot2d1_2 (char *fname, unsigned long fname_len)
{
    return sci_plot2d1_G("plot2d2", 2, fname_len); /* NG */
}
/*--------------------------------------------------------------------------*/
int sci_plot2d1_3 (char *fname, unsigned long fname_len)
{
    return sci_plot2d1_G("plot2d3", 3, fname_len); /* NG */
}
/*--------------------------------------------------------------------------*/
int sci_plot2d1_4 (char *fname, unsigned long fname_len)
{
    return sci_plot2d1_G("plot2d4", 4, fname_len); /* NG */
}
/*--------------------------------------------------------------------------*/
int sci_plot2d1_G(char * fname, int ptype, unsigned long fname_len)
{
    SciErr sciErr;
    int* piAddrl1 = NULL;
    double* l1 = NULL;
    int* piAddrl2 = NULL;
    double* l2 = NULL;
    double* lt = NULL;

    int frame_def = 8;
    int *frame = &frame_def;
    int axes_def = 1;
    int *axes = &axes_def;
    int iskip = 0, test  = 0;
    int m1 = 0, n1 = 0, m2 = 0, n2 = 0, i = 0, j = 0;

    static rhs_opts opts[] =
    {
        { -1, "axesflag", -1, 0, 0, NULL},
        { -1, "frameflag", -1, 0, 0, NULL},
        { -1, "leg", -1, 0, 0, NULL},
        { -1, "logflag", -1, 0, 0, NULL},
        { -1, "nax", -1, 0, 0, NULL},
        { -1, "rect", -1, 0, 0, NULL},
        { -1, "strf", -1, 0, 0, NULL},
        { -1, "style", -1, 0, 0, NULL},
        { -1, NULL, -1, 0, 0, NULL}
    };

    int    * style    = NULL ;
    double* rect     = NULL ;
    int    * nax      = NULL ;
    BOOL     flagNax  = FALSE;
    char   * strf     = NULL ;
    char strfl[4];
    char   * legend   = NULL ;
    char   * logFlags = NULL ;

    if (nbInputArgument(pvApiCtx) <= 0)
    {
        /* lauch the default routines depending on the name of the calling function */
        sci_demo(fname, fname_len);
        return 0;
    }
    CheckInputArgument(pvApiCtx, 1, 9); /* to allow plot2dxx(y) */


    iskip = 0;
    if (getOptionals(pvApiCtx, fname, opts) == 0)
    {
        ReturnArguments(pvApiCtx);
        return 0;
    }

    if (checkInputArgumentType(pvApiCtx, 1, sci_strings))
    {
        /* logflags */
        GetLogflags(pvApiCtx, fname, 1, opts, &logFlags);
        iskip = 1;
    }

    /* added to support plot2dxx([logflags],y) */
    if (nbInputArgument(pvApiCtx) == 1 + iskip)
    {
        if (FirstOpt() <= nbInputArgument(pvApiCtx))
        {
            Scierror(999, _("%s: Misplaced optional argument: #%d must be at position %d.\n"), fname, 1, 3 + iskip);
            return (0);
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, 1 + iskip, &piAddrl2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 1 + iskip.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrl2, &m2, &n2, &l2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1 + iskip);
            return 1;
        }

        sciErr = allocMatrixOfDouble(pvApiCtx, 2 + iskip, m2, n2, &l1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        if (m2 == 1 && n2 > 1)
        {
            m2 = n2;
            n2 = 1;
        }

        m1 = m2;
        n1 = n2;

        for (i = 0; i < m2 ; ++i)
        {
            for (j = 0 ; j < n2 ;  ++j)
            {
                *(l1 + i + m2 * j) = (double) i + 1;
            }
        }
    }


    if (nbInputArgument(pvApiCtx) >= 2 + iskip)
    {
        if (FirstOpt() < 3 + iskip)
        {
            Scierror(999, _("%s: Misplaced optional argument: #%d must be at position %d.\n"),
                     fname, 1, 3 + iskip);
            return (0);
        }


        /* x */
        sciErr = getVarAddressFromPosition(pvApiCtx, 1 + iskip, &piAddrl1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 1 + iskip.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrl1, &m1, &n1, &l1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1 + iskip);
            return 1;
        }

        if (iskip == 1)
        {
            if (logFlags[0] == 'e')
            {
                m1 = 0;
                n1 = 0;
            }
        }

        /* y */
        sciErr = getVarAddressFromPosition(pvApiCtx, 2 + iskip, &piAddrl2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 2 + iskip.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrl2, &m2, &n2, &l2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2 + iskip);
            return 1;
        }

        /* if (m2 * n2 == 0) { m1 = 0; n1 = 0;}  */

        test = (m1 * n1 == 0) /* x = [] */
               /* x,y vectors of same length */
               || ((m1 == 1 || n1 == 1) && (m2 == 1 || n2 == 1) && (m1 * n1 == m2 * n2))
               || ((m1 == m2) && (n1 == n2)) /* size(x) == size(y) */
               /* x vector size(y)==[size(x),.] */
               || ((m1 == 1 && n1 == m2) || (n1 == 1 && m1 == m2));

        //CheckDimProp
        if (!test)
        {
            Scierror(999, _("%s: Wrong size for input arguments: Incompatible sizes.\n"), fname);
            return 1;
        }


        if (m1 * n1 == 0)
        {
            /* default x=1:n */
            sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m2, n2, &lt);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            if (m2 == 1 && n2 > 1)
            {
                m2 = n2;
                n2 = 1;
            }
            for (i = 0; i < m2 ; ++i)
            {
                for (j = 0 ; j < n2 ;  ++j)
                {
                    *(lt + i + m2 * j) = (double) i + 1;
                }
            }
            m1 = m2;
            n1 = n2;
            l1 = lt;
        }
        else if ((m1 == 1 || n1 == 1) && (m2 != 1 && n2 != 1))
        {
            /* a single x vector for mutiple columns for y */
            sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m2, n2, &lt);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            for (i = 0; i < m2 ; ++i)
            {
                for (j = 0 ; j < n2 ;  ++j)
                {
                    *(lt + i + m2 * j) = l1[i];
                }
            }
            m1 = m2;
            n1 = n2;
            l1 = lt;
        }
        else if ((m1 == 1 && n1 == 1) && (n2 != 1))
        {
            /* a single y row vector  for a single x */
            sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m1, n2, &lt);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            for (j = 0 ; j < n2 ;  ++j)
            {
                lt[j] = *l1;
            }
            n1 = n2;
            l1 = lt;
        }
        else
        {
            if (m2 == 1 && n2 > 1)
            {
                m2 = n2;
                n2 = 1;
            }
            if (m1 == 1 && n1 > 1)
            {
                m1 = n1;
                n1 = 1;
            }
        }
    }

    sciGetStyle(pvApiCtx, fname, 3 + iskip, n1, opts, &style);
    GetStrf(pvApiCtx, fname, 4 + iskip, opts, &strf);
    GetLegend(pvApiCtx, fname, 5 + iskip, opts, &legend);
    GetRect(pvApiCtx, fname, 6 + iskip, opts, &rect);
    GetNax(pvApiCtx, 7 + iskip, opts, &nax, &flagNax);
    if (iskip == 0)
    {
        GetLogflags(pvApiCtx, fname, 8, opts, &logFlags);
    }

    if (isDefStrf(strf))
    {
        strcpy(strfl, DEFSTRFN);

        strf = strfl;
        if (!isDefRect(rect))
        {
            strfl[1] = '7';
        }
        if (!isDefLegend(legend))
        {
            strfl[0] = '1';
        }
        GetOptionalIntArg(pvApiCtx, fname, 9, "frameflag", &frame, 1, opts);
        if (frame != &frame_def)
        {
            strfl[1] = (char)(*frame + 48);
        }
        GetOptionalIntArg(pvApiCtx, fname, 9, "axesflag", &axes, 1, opts);
        if (axes != &axes_def)
        {
            strfl[2] = (char)(*axes + 48);
        }
    }

    if (ptype == 0)
    {
        ptype = 1;
    }

    Objplot2d (ptype, logFlags, (l1), (l2), &n1, &m1, style, strf, legend, rect, nax, flagNax);

    // Allocated by sciGetStyle (get_style_arg function in GetCommandArg.c)
    FREE(style);

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
