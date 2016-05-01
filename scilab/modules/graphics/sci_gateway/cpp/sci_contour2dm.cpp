#include "ContourOnMesh.hxx"

extern "C"
{
#include "gw_graphics.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
#include "math_graphics.h"
}

int sci_contour2dm(char * fname, void *pvApiCtx)
{
    SciErr sciErr;
    int m1 = 0, n1 = 0, m2 = 0, n2 = 0, m3 = 0, n3 = 0, m4 = 0, n4 = 0, m5 = 0, n5 = 0, mn1 = 0;
    int iType1 = 0, iType2 = 0, iType3 = 0, iType4 = 0, iType5 = 0;

    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int* piAddr3 = NULL;
    int* piAddr4 = NULL;
    int* piAddr5 = NULL;

    double* l1 = NULL;
    double* l2 = NULL;
    double* l3 = NULL;
    double* l4 = NULL;
    double* l5 = NULL;

    bool l5alloc = false;

    std::vector<double> xC;
    std::vector<double> yC;

    CheckInputArgument(pvApiCtx, 5, 5);
    CheckOutputArgument(pvApiCtx, 2, 2);

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }
    //check type
    sciErr = getVarType(pvApiCtx, piAddr1, &iType1);
    if (sciErr.iErr || iType1 != sci_matrix)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
        printError(&sciErr, 0);
        return 1;
    }
    // Retrieve a matrix of double at position 1.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &m1, &n1, &l1);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
        printError(&sciErr, 0);
        return 1;
    }

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }
    //check type
    sciErr = getVarType(pvApiCtx, piAddr2, &iType2);
    if (sciErr.iErr || iType2 != sci_matrix)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
        printError(&sciErr, 0);
        return 1;
    }
    // Retrieve a matrix of double at position 2.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &m2, &n2, &l2);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
        printError(&sciErr, 0);
        return 1;
    }

    //CheckSameDims
    if (m1 != m2 || n1 != n2)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), fname, 2, m1, n1);
        return 1;
    }

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }
    //check type
    sciErr = getVarType(pvApiCtx, piAddr3, &iType3);
    if (sciErr.iErr || iType3 != sci_matrix)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 3);
        printError(&sciErr, 0);
        return 1;
    }
    // Retrieve a matrix of double at position 3.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &m3, &n3, &l3);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 3);
        printError(&sciErr, 0);
        return 1;
    }

    if (n3 < 5)
    {
        Scierror(999, _("%s: Wrong number of columns for input argument #%d: at least %d expected.\n"), fname, 3, 5);
        return 0;
    }

    // remove number and flag
    n3 -= 2;

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }
    //check type
    sciErr = getVarType(pvApiCtx, piAddr4, &iType4);
    if (sciErr.iErr || iType4 != sci_matrix)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 4);
        printError(&sciErr, 0);
        return 1;
    }
    // Retrieve a matrix of double at position 4.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &m4, &n4, &l4);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 4);
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }
    //check type
    sciErr = getVarType(pvApiCtx, piAddr5, &iType5);
    if (sciErr.iErr || iType5 != sci_matrix)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 5);
        printError(&sciErr, 0);
        return 1;
    }
    // Retrieve a matrix of double at position 5.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr5, &m5, &n5, &l5);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 5);
        printError(&sciErr, 0);
        return 1;
    }

    if (m1 * n1 == 0 || m3 == 0)
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    // If z is a scalar
    if (m5 * n5 == 1)
    {
        double zmin = (double) Mini(l4, m4 * n4);
        double zmax = (double) Maxi(l4, m4 * n4);
        int nb = l5[0];
        l5 = new double[nb];
        l5alloc = true;
        for (int i = 0; i < nb; ++i)
        {
            l5[i] = zmin + (i + 1) * (zmax - zmin) / (nb + 1);
        }
        m5 = 1;
        n5 = nb;
    }

    bool res = org_modules_graphics::LevelCurve::compute(m1 * n1, l1, l2, l4, m3, n3, l3, m5 * n5, l5, xC, yC);
    if (l5alloc)
    {
        delete[] l5;
    }

    if (!res)
    {
        // error in index
        Scierror(999, _("%s: Invalid vertex index.\n"), fname, 3);
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = createMatrixOfDouble(pvApiCtx, 6, 1, xC.size(), &(xC[0]));
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = createMatrixOfDouble(pvApiCtx, 7, 1, yC.size(), &(yC[0]));
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = 6;
    AssignOutputVariable(pvApiCtx, 2) = 7;
    ReturnArguments(pvApiCtx);

    return 0;

}
