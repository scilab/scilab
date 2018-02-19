//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// Copyright (C) 2018 - ESI Group - Clement DAVID
//
// This file is distributed under the same license as the Scilab package.
//

if haveacompiler() then

    //           CALLING EXTERNAL FORTRAN SUBROUTINE

    f1f = ["      subroutine barf(a,b,c)";
    "      double precision  a,b,c";
    "      c=exp(a*log(1+b))-1 ";
    "      return";
    "      end"];

    i=["#include <stdlib.h>"
    "#include <api_scilab.h>"
    "#include <Scierror.h>"
    "#include <localization.h>"
    ""
    "extern int C2F(barf)(double *x, double *y, double *z);"
    ""
    "int sci_barf(char *fname, void* pvApiCtx)"
    "{"
    "  SciErr sciErr;"
    ""
    "  int m1 = 0, n1 = 0;"
    "  double *pdVarOne = NULL;"
    "  int *piAddressVarOne = NULL;"
    "  int m2 = 0, n2 = 0;"
    "  double *pdVarTwo = NULL;"
    "  int *piAddressVarTwo = NULL;"
    "  double *pdblOut = NULL;"
    ""
    "  CheckInputArgument(pvApiCtx, 2, 2);"
    "  CheckOutputArgument(pvApiCtx, 0, 1);"
    ""
    "  sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);"
    "  if(sciErr.iErr)"
    "  {"
    "    printError(&sciErr, 0);"
    "    return 0;"
    "  }"
    ""
    "  sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne, &m1, &n1, &pdVarOne);"
    "  if(sciErr.iErr)"
    "  {"
    "    printError(&sciErr, 0);"
    "    return 0;"
    "  }"
    ""
    "  sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);"
    "  if(sciErr.iErr)"
    "  {"
    "    printError(&sciErr, 0);"
    "    return 0;"
    "  }"
    ""
    "  sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo, &m2, &n2, &pdVarTwo);"
    "  if(sciErr.iErr)"
    "  {"
    "    printError(&sciErr, 0);"
    "    return 0;"
    "  }"
    ""
    "  sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, 1, &pdblOut);"
    "  if (sciErr.iErr)"
    "  {"
    "    printError(&sciErr, 0);"
    "    return 0;"
    "  }"
    ""
    "  C2F(barf)(pdVarOne, pdVarTwo, pdblOut);"
    ""
    "  AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;"
    "  ReturnArguments(pvApiCtx);"
    "  return 0;"
    "}"]

    mprintf("\n");
    mprintf(gettext("Calling a Fortran subroutine from Scilab.\n"));

    disp(f1f);

    // we use TMPDIR for compilation

    if ~c_link("barf") then
        path = pwd();
        chdir(TMPDIR);
        mputl(f1f,"barf.f");
        mputl(i,"sci_barf.c");

        mprintf("\n");
        mprintf(gettext("Calling ilib_for_link to build a Fortran subroutine.\n"));

        lib_ = ilib_for_link(["barf"], "barf.f", [],"f");
        link(lib_, "barf", "f");
        ilib_build("gw_barf",["barf" "sci_barf"],"sci_barf.c",basename(lib_));
        exec loader.sce ;
        chdir(path)
    end

    //Z = X+Y by fortran subroutine
    X = 5;
    Y = 7;

    mprintf("\n");
    mprintf(gettext("Calling Fortran subroutine. Z = (1+Y)**X - 1"));
    mprintf("\n");
    mprintf(gettext("with X = %d"), X);
    mprintf("\n");
    mprintf(gettext("with Y = %d"), Y);
    mprintf("\n");
    mprintf("Z = barf(X, Y);");
    mprintf("\n");
    Z = barf(X, Y);
    mprintf(gettext("Result Z = %d"), Z);
    mprintf("\n");

end
