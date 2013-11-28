/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - INRIA - Pierre Lando
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: GetCommandArg.h                                                  */
/* desc : tools to retrieve parameters within the command line for        */
/*        graphic routines.                                               */
/*------------------------------------------------------------------------*/

#include "GetCommandArg.h"
#include "GetProperty.h"
#include "DefaultCommandArg.h"
#include "CurrentSubwin.h"
#include "localization.h"
#include "Scierror.h"
#include "BuildObjects.h"
#include "api_scilab.h"
#include "MALLOC.h"

static char logFlagsCpy[3] ; /* real logflags may use either this or the stack */

/*--------------------------------------------------------------------------*/
/* get_style */
/*--------------------------------------------------------------------------*/
int get_style_arg(void* _pvCtx, char *fname, int pos, int n1, rhs_opts opts[], int ** style)
{
    int m = 0, n = 0, first_opt = FirstOpt(), kopt = 0, un = 1, ix = 0, i = 0, l1 = 0;

    if ( pos < first_opt ) /* regular argument  */
    {
        int* piAddr = 0;
        int iType = 0;
        int* piData = NULL;
        getVarAddressFromPosition(_pvCtx, pos, &piAddr);
        getVarType(_pvCtx, piAddr, &iType);

        if (iType)
        {
            getMatrixOfDoubleAsInteger(_pvCtx, piAddr, &m, &n, &piData);
            if (m * n < n1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: %d < %d expected.\n"), fname, pos, m * n, n1);
                return 0;
            }

            if ( n1 == 1 && m * n == 1 )
            {
                *style = (int*)MALLOC(2 * sizeof(int));
                (*style)[0] = piData[0];
                (*style)[1] = 1;
            }
            else
            {
                *style = (int*)MALLOC(m * n * sizeof(int));
                for (i = 0; i < m * n; i++)
                {
                    (*style)[i] = piData[i];
                }
            }
        }
        else /* zero type argument --> default value */
        {
            ix = Max(n1, 2);
            *style = (int*)MALLOC(ix * sizeof(int));
            (*style)[1] = 1;

            for ( i = 0 ; i < n1 ; ++i )
            {
                (*style)[i] = i + 1;
            }
        }
    }
    else if ((kopt = FindOpt("style", opts)))
    {
        /* optinal argument: style=value */
        int* piAddr = 0;
        int* piData = NULL;

        getVarAddressFromPosition(_pvCtx, kopt, &piAddr);
        getMatrixOfDoubleAsInteger(_pvCtx, piAddr, &m, &n, &piData);
        if (m * n < n1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: %d < %d expected.\n"), fname, kopt, m * n, n1);
            return 0;
        }

        if (n1 == 1 && m * n == 1)
        {
            *style = (int*)MALLOC(2 * sizeof(int));
            (*style)[0] = piData[0];
            (*style)[1] = 1;
        }
        else
        {
            *style = (int*)MALLOC(m * n * sizeof(int));
            for (i = 0; i < m * n; i++)
            {
                (*style)[i] = piData[i];
            }
        }
    }
    else /* unspecified argument --> default value */
    {
        ix = Max(n1, 2);
        *style = (int*)MALLOC(ix * sizeof(int));
        (*style)[1] = 1;

        for (i = 0 ; i < n1 ; ++i)
        {
            (*style)[i] = i + 1;
        }
    }

    return 1;
}
/*--------------------------------------------------------------------------*/
/* get_rect */
/*--------------------------------------------------------------------------*/
int get_rect_arg(void* _pvCtx, char *fname, int pos, rhs_opts opts[], double ** rect)
{
    int m, n, first_opt = FirstOpt(), kopt, i;

    if (pos < first_opt)
    {
        int* piAddr = 0;
        int iType = 0;
        double* pdblData = NULL;
        getVarAddressFromPosition(_pvCtx, pos, &piAddr);
        getVarType(_pvCtx, piAddr, &iType);

        if (iType)
        {
            getMatrixOfDouble(_pvCtx, piAddr, &m, &n, &pdblData);
            if (m * n != 4)
            {
                Scierror(999, "%s: Wrong size for input argument #%d: %d expected\n", fname, pos, 4);
                return 0;
            }

            *rect = pdblData;

            for (i = 0; i < 4; i++)
            {
                if (finite((*rect)[i]) == 0)
                {
                    Scierror(999, "%s: Wrong values (Nan or Inf) for input argument: %d finite values expected\n", fname, 4);
                    return 0;
                }
            }
        }
        else
        {
            /** global value can be modified  **/
            double zeros[4] = { 0.0, 0.0, 0.0, 0.0 };
            setDefRect(zeros);
            *rect = getDefRect();
        }
    }
    else if ((kopt = FindOpt("rect", opts))) /* named argument: rect=value */
    {
        int* piAddr = 0;
        double* pdblData = NULL;
        getVarAddressFromPosition(_pvCtx, kopt, &piAddr);
        getMatrixOfDouble(_pvCtx, piAddr, &m, &n, &pdblData);
        if (m * n != 4)
        {
            Scierror(999, "%s: Wrong size for input argument #%d: %d expected\n", fname, kopt, 4);
            return 0;
        }

        *rect = pdblData;

        for (i = 0; i < 4; i++)
        {
            if (finite((*rect)[i]) == 0)
            {
                Scierror(999, "%s: Wrong values (Nan or Inf) for input argument: %d finite values expected\n", fname, 4);
                return 0;
            }
        }
    }
    else
    {
        /** global value can be modified  **/
        double zeros[4] = { 0.0, 0.0, 0.0, 0.0 };
        setDefRect(zeros);
        *rect = getDefRect();
    }

    return 1;
}
/*--------------------------------------------------------------------------*/
int get_strf_arg(void* _pvCtx, char *fname, int pos, rhs_opts opts[], char ** strf)
{
    int first_opt = FirstOpt(), kopt;

    if (pos < first_opt)
    {
        int* piAddr = 0;
        int iType = 0;
        char* pstData = NULL;
        getVarAddressFromPosition(_pvCtx, pos, &piAddr);
        getVarType(_pvCtx, piAddr, &iType);

        if (iType)
        {
            getAllocatedSingleString(_pvCtx, piAddr, &pstData);
            if ((int)strlen(pstData) != 3)
            {
                freeAllocatedSingleString(pstData);
                Scierror(999, _("%s: Wrong size for input argument #%d: String of %d characters expected.\n"), fname, pos, 3);
                return 0;
            }
            *strf = pstData;
        }
        else
        {
            /* def value can be changed */
            reinitDefStrf();
            *strf = getDefStrf();
        }
    }
    else if ((kopt = FindOpt("strf", opts)))
    {
        int* piAddr = 0;
        char* pstData = NULL;
        getVarAddressFromPosition(_pvCtx, kopt, &piAddr);
        getAllocatedSingleString(_pvCtx, piAddr, &pstData);
        if ((int)strlen(pstData) != 3)
        {
            freeAllocatedSingleString(pstData);
            Scierror(999, _("%s: Wrong size for input argument #%d: String of %d characters expected.\n"), fname, kopt, 3);
            return 0;
        }
        *strf = pstData;
    }
    else
    {
        /* def value can be changed */
        reinitDefStrfN();
        *strf = getDefStrf();
    }
    return 1;
}

/*--------------------------------------------------------------------------*/
int get_legend_arg(void* _pvCtx, char *fname, int pos, rhs_opts opts[], char ** legend)
{
    int first_opt = FirstOpt(), kopt;

    if (pos < first_opt)
    {
        int* piAddr = 0;
        int iType = 0;
        char* pstData = NULL;
        getVarAddressFromPosition(_pvCtx, pos, &piAddr);
        getVarType(_pvCtx, piAddr, &iType);

        if (iType)
        {
            getAllocatedSingleString(_pvCtx, piAddr, &pstData);
            *legend = pstData;
        }
        else
        {
            *legend = getDefLegend();
        }
    }
    else if ((kopt = FindOpt("leg", opts)))
    {
        int* piAddr = 0;
        char* pstData = NULL;
        getVarAddressFromPosition(_pvCtx, kopt, &piAddr);
        getAllocatedSingleString(_pvCtx, piAddr, &pstData);
        *legend = pstData;
    }
    else
    {
        *legend = getDefLegend();
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
/**
 * retrieve the labels from the command line and store them into labels
 */
int get_labels_arg(void* _pvCtx, char *fname, int pos, rhs_opts opts[], char ** labels)
{
    int first_opt = FirstOpt(), kopt;

    if (pos < first_opt)
    {
        int* piAddr = 0;
        int iType = 0;
        char* pstData = NULL;
        getVarAddressFromPosition(_pvCtx, pos, &piAddr);
        getVarType(_pvCtx, piAddr, &iType);

        if (iType)
        {
            getAllocatedSingleString(_pvCtx, piAddr, &pstData);
            *labels = pstData;
        }
        else
        {
            /* jb silvy 03/2006 */
            /* do not change the legend if one already exists */
            if (sciGetLegendDefined(getOrCreateDefaultSubwin()))
            {
                *labels = NULL;
            }
            else
            {
                *labels = getDefLegend();
            }
        }
    }
    else if ((kopt = FindOpt("leg", opts)))
    {
        int* piAddr = 0;
        char* pstData = NULL;
        getVarAddressFromPosition(_pvCtx, kopt, &piAddr);
        getAllocatedSingleString(_pvCtx, piAddr, &pstData);
        *labels = pstData;
    }
    else
    {
        /* jb silvy 03/2006 */
        /* do not change the legend if one already exists */
        if (sciGetLegendDefined(getOrCreateDefaultSubwin()))
        {
            *labels = NULL;
        }
        else
        {
            *labels = getDefLegend();
        }
    }
    return 1;
}

/*--------------------------------------------------------------------------*/
int get_nax_arg(void* _pvCtx, int pos, rhs_opts opts[], int ** nax, BOOL * flagNax)
{
    int i, m, n, first_opt = FirstOpt(), kopt;

    if (pos < first_opt)
    {
        int* piAddr = 0;
        int iType = 0;
        int* piData = NULL;
        getVarAddressFromPosition(_pvCtx, pos, &piAddr);
        getVarType(_pvCtx, piAddr, &iType);

        if (iType)
        {
            getMatrixOfDoubleAsInteger(_pvCtx, piAddr, &m, &n, &piData);
            if (n * m != 4)
            {
                return 1;
            }

            for (i = 0 ; i < 4; ++i)
            {
                // When i = 1 or 3 we talk about the number of ticks, this value can be -1 to say 'AutoTicks'
                piData[i] = Max(piData[i], -(i % 2));
            }
            *nax = piData;
            *flagNax = TRUE;
        }
        else
        {
            *nax = getDefNax();
            *flagNax = FALSE;
        }
    }
    else if ((kopt = FindOpt("nax", opts)))
    {
        int* piAddr = 0;
        int* piData = NULL;

        getVarAddressFromPosition(_pvCtx, kopt, &piAddr);
        getMatrixOfDoubleAsInteger(_pvCtx, piAddr, &m, &n, &piData);
        if (m * n != 4)
        {
            return 1;
        }

        for (i = 0 ; i < 4; ++i)
        {
            // When i = 1 or 3 we talk about the number of ticks, this value can be -1 to say 'AutoTicks'
            piData[i] = Max(piData[i], -(i % 2));
        }
        *nax = piData;
        *flagNax = TRUE;
    }
    else
    {
        *nax = getDefNax();
        *flagNax = FALSE;
    }
    return 1;
}


/*--------------------------------------------------------------------------*/
int get_zminmax_arg(void* _pvCtx, char *fname, int pos, rhs_opts opts[], double ** zminmax)
{
    int m, n, first_opt = FirstOpt(), kopt;

    if (pos < first_opt)
    {
        int* piAddr = 0;
        int iType = 0;
        double* pdblData = NULL;
        getVarAddressFromPosition(_pvCtx, pos, &piAddr);
        getVarType(_pvCtx, piAddr, &iType);

        if (iType)
        {
            getMatrixOfDouble(_pvCtx, piAddr, &m, &n, &pdblData);
            if (m * n != 2)
            {
                Scierror(999, "%s: Wrong size for input argument #%d: %d expected\n", fname, pos, 2);
                return 0;
            }
            *zminmax = pdblData;
        }
        else
        {
            /** global value can be modified  **/
            double zeros[2] = { 0.0, 0.0 };
            setDefZminMax(zeros);
            *zminmax = getDefZminMax();
        }
    }
    else if ((kopt = FindOpt("zminmax", opts))) /* named argument: rect=value */
    {
        int* piAddr = 0;
        double* pdblData = NULL;
        getVarAddressFromPosition(_pvCtx, kopt, &piAddr);
        getMatrixOfDouble(_pvCtx, piAddr, &m, &n, &pdblData);
        if (m * n != 2)
        {
            Scierror(999, "%s: Wrong size for input argument #%d: %d expected\n", fname, kopt, 2);
            return 0;
        }
        *zminmax = pdblData;
    }
    else
    {
        /** global value can be modified  **/
        double zeros[2] = { 0.0, 0.0 };
        setDefZminMax(zeros);
        *zminmax = getDefZminMax();
    }

    return 1;
}

/*--------------------------------------------------------------------------*/
int get_colminmax_arg(void* _pvCtx, char *fname, int pos, rhs_opts opts[], int ** colminmax)
{
    int m, n, first_opt = FirstOpt(), kopt;

    if (pos < first_opt)
    {
        int* piAddr = 0;
        int iType = 0;
        int* piData = NULL;
        getVarAddressFromPosition(_pvCtx, pos, &piAddr);
        getVarType(_pvCtx, piAddr, &iType);

        if (iType)
        {
            getMatrixOfDoubleAsInteger(_pvCtx, piAddr, &m, &n, &piData);
            if (m * n != 2)
            {
                return 1;
            }
            *colminmax = piData;
        }
        else
        {
            /** global value can be modified  **/
            int zeros[2] = { 0, 0 };
            setDefColMinMax(zeros);
            *colminmax = getDefColMinMax();
        }
    }
    else if ((kopt = FindOpt("colminmax", opts)))
    {
        int* piAddr = 0;
        int* piData = NULL;

        getVarAddressFromPosition(_pvCtx, kopt, &piAddr);
        getMatrixOfDoubleAsInteger(_pvCtx, piAddr, &m, &n, &piData);
        if (m * n != 2)
        {
            return 1;
        }
        *colminmax = piData;
    }
    else
    {
        /** global value can be modified  **/
        int zeros[2] = { 0, 0 };
        setDefColMinMax(zeros);
        *colminmax = getDefColMinMax();
    }
    return 1;
}

/*--------------------------------------------------------------------------*/
int get_colout_arg(void* _pvCtx, char *fname, int pos, rhs_opts opts[], int ** colout)
{
    int m, n, first_opt = FirstOpt(), kopt;

    if (pos < first_opt)
    {
        int* piAddr = 0;
        int iType = 0;
        int* piData = NULL;
        getVarAddressFromPosition(_pvCtx, pos, &piAddr);
        getVarType(_pvCtx, piAddr, &iType);

        if (iType)
        {
            getMatrixOfDoubleAsInteger(_pvCtx, piAddr, &m, &n, &piData);
            if (m * n != 2)
            {
                return 1;
            }
            *colout = piData;
        }
        else
        {
            /** global value can be modified  **/
            int newDefCO[2] = { -1, -1 };
            setDefColOut(newDefCO);
            *colout = getDefColOut();
        }
    }
    else if ((kopt = FindOpt("colout", opts)))
    {
        int* piAddr = 0;
        int* piData = NULL;

        getVarAddressFromPosition(_pvCtx, kopt, &piAddr);
        getMatrixOfDoubleAsInteger(_pvCtx, piAddr, &m, &n, &piData);
        if (m * n != 2)
        {
            return 1;
        }
        *colout = piData;
    }
    else
    {
        /** global value can be modified  **/
        int newDefCO[2] = { -1, -1 };
        setDefColOut(newDefCO);
        *colout = getDefColOut();
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
int get_with_mesh_arg(void* _pvCtx, char *fname, int pos, rhs_opts opts[], BOOL * withMesh)
{
    int first_opt = FirstOpt(), kopt;

    if (pos < first_opt)
    {
        int* piAddr = 0;
        int iType = 0;
        int iData = 0;
        getVarAddressFromPosition(_pvCtx, pos, &piAddr);
        getVarType(_pvCtx, piAddr, &iType);

        if (iType)
        {
            getScalarBoolean(_pvCtx, piAddr, &iData);
            *withMesh = iData;
        }
        else
        {
            /** global value can be modified  **/
            setDefWithMesh(FALSE);
            *withMesh = getDefWithMesh();
        }
    }
    else if ((kopt = FindOpt("mesh", opts)))
    {
        int* piAddr = 0;
        int iData = 0;
        getVarAddressFromPosition(_pvCtx, kopt, &piAddr);

        getScalarBoolean(_pvCtx, piAddr, &iData);
        *withMesh = iData;
    }
    else
    {
        /** global value can be modified  **/
        setDefWithMesh(FALSE);
        *withMesh = getDefWithMesh();
    }
    return 1;
}

/*--------------------------------------------------------------------------*/
int get_logflags_arg(void* _pvCtx, char *fname, int pos, rhs_opts opts[], char ** logFlags)
{
    int kopt = 0;
    int* piAddr = NULL;

    int iLog = 0;
    char* pstLog = NULL;
    if (pos < FirstOpt()) //input argument  */
    {
        //no idea of the real goal of this, how input var can have type == 0 Oo
        if (getInputArgumentType(_pvCtx, pos) == 0)
        {
            *logFlags = getDefLogFlags();
            return 1;
        }

        getVarAddressFromPosition(_pvCtx, pos, &piAddr);
    }
    else if ((kopt = FindOpt("logflag", opts))) //optional argument
    {
        getVarAddressFromPosition(_pvCtx, kopt, &piAddr);
    }
    else
    {
        //take default value
        *logFlags = getDefLogFlags();
        return 1;
    }

    getAllocatedSingleString(_pvCtx, piAddr, &pstLog);
    iLog = (int)strlen(pstLog);
    if (iLog != 2 && iLog != 3)
    {
        Scierror(999, "%s: Wrong size for input argument #%d: %d or %d expected\n", fname, pos, 2, 3);
        return 0;
    }

    if (iLog == 2)
    {
        if ((pstLog[0] != 'l' && pstLog[0] != 'n') || (pstLog[1] != 'l' && pstLog[1] != 'n'))
        {
            Err = pos;
            SciError(116);
            return 0;
        }

        logFlagsCpy[0] = 'g';
        logFlagsCpy[1] = pstLog[0];
        logFlagsCpy[2] = pstLog[1];
        *logFlags = logFlagsCpy;
    }
    else //iLog == 3
    {
        if (((pstLog[0] != 'g') && (pstLog[0] != 'e') && (pstLog[0] != 'o')) ||
                (pstLog[1] != 'l' && pstLog[1] != 'n') ||
                (pstLog[2] != 'l' && pstLog[2] != 'n'))
        {
            Err = pos;
            SciError(116);
            return 0;
        }

        *logFlags = pstLog;
    }

    return 1;
}
/*--------------------------------------------------------------------------*/
int get_optional_double_arg(void* _pvCtx, char* fname, int pos, char* name, double** value, int sz, rhs_opts opts[])
{
    int m, n, first_opt = FirstOpt(), kopt;

    if (pos < first_opt)
    {
        int* piAddr = 0;
        int iType = 0;
        double* pdblData = NULL;
        getVarAddressFromPosition(_pvCtx, pos, &piAddr);
        getVarType(_pvCtx, piAddr, &iType);

        if (iType)
        {
            getMatrixOfDouble(_pvCtx, piAddr, &m, &n, &pdblData);
            if (m * n != sz)
            {
                return 1;
            }
            *value = pdblData;
        }
    }
    else if ((kopt = FindOpt(name, opts)))
    {
        int* piAddr = 0;
        double* pdblData = NULL;
        getVarAddressFromPosition(_pvCtx, kopt, &piAddr);
        getMatrixOfDouble(_pvCtx, piAddr, &m, &n, &pdblData);
        if (m * n != sz)
        {
            Scierror(999, "%s: Wrong size for input argument #%d: %d expected\n", fname, kopt, 4);
            return 0;
        }

        *value = pdblData;
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
int get_optional_int_arg(void* _pvCtx, char* fname, int pos, char* name, int** value, int sz, rhs_opts opts[])
{
    int m, n, first_opt = FirstOpt(), kopt;

    if (pos < first_opt)
    {
        int* piAddr = 0;
        int iType = 0;
        int* piData = NULL;
        getVarAddressFromPosition(_pvCtx, pos, &piAddr);
        getVarType(_pvCtx, piAddr, &iType);

        if (iType)
        {
            getMatrixOfDoubleAsInteger(_pvCtx, piAddr, &m, &n, &piData);
            if (m * n != sz)
            {
                return 1;
            }
            *value = piData;
        }
    }
    else if ((kopt = FindOpt(name, opts)))
    {
        int* piAddr = 0;
        int* piData = NULL;

        getVarAddressFromPosition(_pvCtx, kopt, &piAddr);
        getMatrixOfDoubleAsInteger(_pvCtx, piAddr, &m, &n, &piData);
        if (m * n < 1)
        {
            return 1;
        }
        *value = piData;
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
