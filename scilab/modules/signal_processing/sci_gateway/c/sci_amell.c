/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
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

#include "gw_signal.h"

#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"

extern int C2F(amell)(double *du, double *dk, double *dsn2, int *n);

int sci_amell(char *fname, void* pvApiCtx)
{
    int input_rows, input_cols, output_length, rows, cols;
    double *argument, *input, *output;
    double parameter;
    int *arg;

    CheckRhs(2, 2);
    CheckLhs(1, 1);
    getVarAddressFromPosition(pvApiCtx, 1, &arg);
    if (isVarComplex(pvApiCtx, arg))
    {
        Scierror(999, _("%s: Input argument #%d must be real.\n"), fname, 1);
        return 1;
    }
    getMatrixOfDouble(pvApiCtx, arg, &input_rows, &input_cols, &input);
    getVarAddressFromPosition(pvApiCtx, 2, &arg);
    if (isVarComplex(pvApiCtx, arg))
    {
        Scierror(999, _("%s: Input argument #%d must be real.\n"), fname, 2);
        return 1;
    }
    getMatrixOfDouble(pvApiCtx, arg, &rows, &cols, &argument);
    parameter = *argument;
    if (rows != 1 || cols != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 2);
        return 1;
    }
    if (parameter < 0 || parameter > 1)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%s, %s].\n"), fname, "0", "1");
        return 1;
    }
    allocMatrixOfDouble(pvApiCtx, Rhs + 1, input_rows, input_cols, &output);
    output_length = input_rows * input_cols;
    C2F(amell)(input, &parameter, output, &output_length);
    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
