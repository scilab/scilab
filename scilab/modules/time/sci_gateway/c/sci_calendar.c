
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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

/*--------------------------------------------------------------------------*/
#include "gw_time.h"
#include "api_scilab.h"
#include "sci_malloc.h"
#include "Scierror.h"
#include "transposeMatrix.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
static int isBissextile (unsigned year);
static unsigned months_to_days (unsigned month);
static long years_to_days (unsigned year);
static long ymd_to_scalar (unsigned year, unsigned month, unsigned day);
/*--------------------------------------------------------------------------*/
#define NBRDAY 7
#define NBRWEEK 6
/*--------------------------------------------------------------------------*/
int days[12]    = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
/*--------------------------------------------------------------------------*/
int sci_calendar(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int n1 = 0, m1 = 0;
    int * p1_in_address = NULL;
    double dblReal = 0;

    int month = 0;
    int year = 0;
    int day, day_1, numdays, i;
    int a = 0;

    double *CALMONTH  = NULL;
    double *tmpMatrix = NULL;

    //Rhs = Max(0, Rhs);

    CheckRhs(2, 2);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &p1_in_address);

    if (getScalarDouble(pvApiCtx, p1_in_address, &dblReal))
    {
        Scierror(999, _("%s: Wrong type for input arguments: Scalar values expected.\n"), fname);
        return 0;
    }

    year = (int)dblReal;

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &p1_in_address);
    if (getScalarDouble(pvApiCtx, p1_in_address, &dblReal))
    {
        Scierror(999, _("%s: Wrong type for input arguments: Scalar values expected.\n"), fname);
        return 0;
    }

    month = (int)dblReal;

    if ( (year < 1800) || (year > 3000) )
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"), fname, 2, 1800, 3000);
        return 0;
    }

    if ( (month < 1) || (month > 12) )
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"), fname, 1, 1, 12);
        return 0;
    }

    CALMONTH = (double *)MALLOC( (NBRDAY * NBRWEEK) * sizeof(double) );
    for (i = 0; i < NBRDAY * NBRWEEK; i++)
    {
        CALMONTH[i] = 0;
    }

    /* check if the month of feb is 28 or 29 days */
    numdays = days[month - 1];
    if (2 == month && isBissextile(year))
    {
        ++numdays;
    }

    /* Starts the calendar on monday */
    day_1 = (int)((ymd_to_scalar(year, month, 1) - (long)1) % 7L);

    for (day = 0; day < day_1; ++day)
    {
        a++;
    }

    /* Browse all the days */
    for (day = 1; day <= numdays; ++day, ++day_1, day_1 %= 7)
    {
        CALMONTH[a] = day;
        a++;
    }

    m1 = NBRWEEK;
    n1 = NBRDAY;
    tmpMatrix = CALMONTH;

    CALMONTH = transposeMatrixDouble(NBRDAY, NBRWEEK, CALMONTH);
    if (tmpMatrix)
    {
        FREE(tmpMatrix);
        tmpMatrix = NULL;
    }
    sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m1, n1, CALMONTH);


    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    if (CALMONTH)
    {
        FREE(CALMONTH);
        CALMONTH = NULL;
    }

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
static int isBissextile (unsigned year)
{
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}
/*--------------------------------------------------------------------------*/
static unsigned months_to_days (unsigned month)
{
    return (month * 3057 - 3007) / 100;
}
/*--------------------------------------------------------------------------*/
static long years_to_days (unsigned year)
{
    return year * 365L + year / 4 - year / 100 + year / 400;
}
/*--------------------------------------------------------------------------*/
static long ymd_to_scalar (unsigned year, unsigned month, unsigned day)
{
    long scalaire;
    scalaire = day + months_to_days(month);
    if ( month > 2 )
    {
        scalaire -= isBissextile(year) ? 1 : 2;
    }
    year--;
    scalaire += years_to_days(year);
    return scalaire;
}
/*--------------------------------------------------------------------------*/

