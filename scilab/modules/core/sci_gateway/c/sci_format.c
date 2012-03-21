/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_core.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
#define e_type_format "e"
#define v_type_format "v"
#define format_MIN 2
#define format_MAX 25
#define format_e_MIN 8
#define mode_variable 1
#define mode_e 0
/*--------------------------------------------------------------------------*/
static int sci_format_tworhs(char *fname);
static int sci_format_onerhs(char *fname);
static int sci_format_norhs(char *fname);
static void setVariableFormat(int numberDigits);
static void set_e_Format(int numberDigits);
static void getFormat(double *e_mode, double *numberDigits);
/*--------------------------------------------------------------------------*/
int C2F(sci_format) (char *fname, unsigned long fname_len)
{
    Rhs = Max(0, Rhs);

    CheckRhs(0, 2);
    CheckLhs(0, 1);

    switch (Rhs)
    {
    case 2:
        return sci_format_tworhs(fname);
    case 1:
        return sci_format_onerhs(fname);
        break;
    case 0:
    default:
        return sci_format_norhs(fname);
    }
}

/*--------------------------------------------------------------------------*/
static int sci_format_norhs(char *fname)
{
    SciErr sciErr;
    double dParamout[2];

    getFormat(&dParamout[0], &dParamout[1]);

    sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 2, dParamout);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999,_("%s: Memory allocation error.\n"), fname);
        return 0;
    }
    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
static int sci_format_onerhs(char *fname)
{
    int *piAddressVarOne = NULL;
    SciErr sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarOne))
    {
        char *param = NULL;

        if (!isScalar(pvApiCtx, piAddressVarOne))
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 1, e_type_format, v_type_format);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &param) == 0)
        {
            if ((strcmp(e_type_format, param) == 0) || (strcmp(v_type_format, param) == 0))
            {
                double previous_mode = 0;
                double previous_numberDigits = 0;

                getFormat(&previous_mode, &previous_numberDigits);
                if (strcmp(e_type_format, param) == 0)
                {
                    set_e_Format((int)previous_numberDigits);
                }
                else            /* v_type_format */
                {
                    setVariableFormat((int)previous_numberDigits);
                }

                freeAllocatedSingleString(param);
                param = NULL;

                LhsVar(1) = 0;
                PutLhsVar();
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 1, e_type_format, v_type_format);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
    }
    else if (isDoubleType(pvApiCtx, piAddressVarOne))
    {
        if (isScalar(pvApiCtx, piAddressVarOne))
        {
            double dValue = 0.;

            if (getScalarDouble(pvApiCtx, piAddressVarOne, &dValue) == 0)
            {
                double mode = 0.;
                double previous_numberDigits = 0.;

                unsigned int value = (int)dValue;

                if (dValue != (double)value)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 1);
                    return 0;
                }

                getFormat(&mode, &previous_numberDigits);

                if (mode == mode_e)
                {
                    if ((value < format_e_MIN) || (value > format_MAX))
                    {
                        Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%d, %d].\n"), fname, 1, format_e_MIN,
                                 format_MAX);
                        return 0;
                    }

                    set_e_Format(value);
                }
                else            /* mode_variable */
                {
                    if ((value < format_MIN) || (value > format_MAX))
                    {
                        Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%d, %d].\n"), fname, 1, format_MIN,
                                 format_MAX);
                        return 0;
                    }

                    setVariableFormat(value);
                }
            }
            LhsVar(1) = 0;
            PutLhsVar();
        }
        else
        {
            if (checkVarDimension(pvApiCtx, piAddressVarOne, 1, 2) || checkVarDimension(pvApiCtx, piAddressVarOne, 2, 1))
            {
                int nbRowsOne = 0;
                int nbColsOne = 0;
                double *pDouble = NULL;

                sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne, &nbRowsOne, &nbColsOne, &pDouble);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999,_("%s: Memory allocation error.\n"), fname);
                    return 0;
                }

                if ((pDouble[1] != mode_e) && (pDouble[1] != mode_variable))
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d.\n"), fname, 1);
                    return 0;
                }
                else
                {
                    if (pDouble[1] == mode_e)
                    {
                        if ((pDouble[0] < format_e_MIN) || (pDouble[0] > format_MAX))
                        {
                            Scierror(999, _("%s: Wrong value for input argument #%d.\n"), fname, 1);
                            return 0;
                        }
                        set_e_Format((int)pDouble[0]);

                    }
                    else        /* mode_variable */
                    {
                        if ((pDouble[0] < format_MIN) || (pDouble[0] > format_MAX))
                        {
                            Scierror(999, _("%s: Wrong value for input argument #%d.\n"), fname, 1);
                            return 0;
                        }
                        setVariableFormat((int)pDouble[0]);
                    }

                    LhsVar(1) = 0;
                    PutLhsVar();
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 1);
                return 0;
            }
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string or a scalar integer value expected.\n"), fname, 1);
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
static int sci_format_tworhs(char *fname)
{
    /* format(1, 10) */
    int *piAddressVarOne = NULL;
    int *piAddressVarTwo = NULL;

    SciErr sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (!isScalar(pvApiCtx, piAddressVarOne))
    {
        Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isScalar(pvApiCtx, piAddressVarTwo))
    {
        Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (isDoubleType(pvApiCtx, piAddressVarOne) && isDoubleType(pvApiCtx, piAddressVarTwo))
    {
        double type_value_d = 0.;
        double v_value_d = 0.;
        int v_value = 0;

        if (getScalarDouble(pvApiCtx, piAddressVarOne, &v_value_d) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        if (getScalarDouble(pvApiCtx, piAddressVarTwo, &type_value_d) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        v_value = (int)v_value_d;

        if ((type_value_d != (double)0) && (type_value_d != (double)1))
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: '0' or '1' expected.\n"), fname, 2);
            return 0;
        }

        if (v_value_d != (double)v_value)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 1);
            return 0;
        }

        if (type_value_d == 1)
        {
            if ((v_value < format_MIN) || (v_value > format_MAX))
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%d, %d].\n"), fname, 1, format_MIN, format_MAX);
                return 0;
            }

            setVariableFormat(v_value);
        }
        else
        {
            if ((v_value < format_e_MIN) || (v_value > format_MAX))
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%d, %d].\n"), fname, 1, format_e_MIN, format_MAX);
                return 0;
            }
            set_e_Format(v_value);
        }

        LhsVar(1) = 0;
        PutLhsVar();
    }
    /* format('e',10) & format(10,'e') syntax */
    else if ((isStringType(pvApiCtx, piAddressVarOne) &&
              (isDoubleType(pvApiCtx, piAddressVarTwo)) || (isDoubleType(pvApiCtx, piAddressVarOne) && isStringType(pvApiCtx, piAddressVarTwo))))
    {
        char *param = NULL;
        unsigned int value = 0;

        if (isStringType(pvApiCtx, piAddressVarOne))
        {
            double dvalue = 0;

            if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &param) != 0)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }

            if (getScalarDouble(pvApiCtx, piAddressVarTwo, &dvalue) != 0)
            {
                freeAllocatedSingleString(param);
                param = NULL;

                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }
            value = (int)dvalue;
            if (dvalue != (double)value)
            {
                freeAllocatedSingleString(param);
                param = NULL;

                Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 2);
                return 0;
            }
        }
        else                    /* matrix */
        {
            double dvalue = 0;

            if (getScalarDouble(pvApiCtx, piAddressVarOne, &dvalue) != 0)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }

            value = (int)dvalue;
            if (dvalue != (double)value)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 1);
                return 0;
            }

            if (getAllocatedSingleString(pvApiCtx, piAddressVarTwo, &param) != 0)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }
        }

        if ((strcmp(e_type_format, param) == 0) || (strcmp(v_type_format, param) == 0))
        {

            if (strcmp(e_type_format, param) == 0)
            {
                freeAllocatedSingleString(param);
                param = NULL;

                if ((value < format_e_MIN) || (value > format_MAX))
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%d, %d].\n"), fname, 2, format_e_MIN,
                             format_MAX);
                    return 0;
                }

                set_e_Format(value);
            }
            else                /* v_type_format */
            {
                freeAllocatedSingleString(param);
                param = NULL;

                if ((value < format_MIN) || (value > format_MAX))
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%d, %d].\n"), fname, 2, format_MIN, format_MAX);
                    return 0;
                }

                setVariableFormat(value);
            }

            LhsVar(1) = 0;
            PutLhsVar();
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 1, e_type_format, v_type_format);
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for inputs arguments.\n"), fname);
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
static void setVariableFormat(int numberDigits)
{
    int numberDigitsAdjusted = numberDigits;

    C2F(iop).lct[5] = mode_variable;    /* set 'v' mode */

    numberDigitsAdjusted = Min(Max(format_MIN, numberDigits), format_MAX);
    C2F(iop).lct[6] = numberDigitsAdjusted;
}

/*--------------------------------------------------------------------------*/
static void set_e_Format(int numberDigits)
{
    int numberDigitsAdjusted = numberDigits;

    C2F(iop).lct[5] = mode_e;   /* set 'e' mode */

    numberDigitsAdjusted = Min(Max(format_MIN, numberDigits), format_MAX);
    numberDigitsAdjusted = Max(numberDigitsAdjusted, format_e_MIN);

    C2F(iop).lct[6] = numberDigitsAdjusted;
}

/*--------------------------------------------------------------------------*/
static void getFormat(double *e_mode, double *numberDigits)
{
    *e_mode = (double)C2F(iop).lct[5];
    *numberDigits = (double)C2F(iop).lct[6];
}

/*--------------------------------------------------------------------------*/
