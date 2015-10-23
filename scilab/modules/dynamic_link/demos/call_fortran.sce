//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
// This file is distributed under the same license as the Scilab package.
//

if haveacompiler() then

    //           CALLING EXTERNAL FORTRAN SUBROUTINE

    f1f = ["      subroutine foof(a,b,c)";
    "      double precision  a,b,c";
    "      c=a+b ";
    "      return"; "       end"];
    
    i=["#include <stdlib.h>"
       "#include <api_scilab.h>"
       "#include <Scierror.h>"
       "#include <localization.h>"
       ""
       "extern int C2F(foof)(double *x, double *y, double *z);"
       ""
       "int sci_foof(char *fname, void* pvApiCtx)"
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
       "  C2F(foof)(pdVarOne, pdVarTwo, pdblOut);"
       ""
       "  AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;"
       "  ReturnArguments(pvApiCtx);"
       "  return 0;"
   "}"]

    mprintf("\n");
    mprintf(gettext("Calling a Fortran subroutine from Scilab.\n"));

    disp(f1f);

    // we use TMPDIR for compilation

    if ~c_link("foof") then
        path = pwd();
        chdir(TMPDIR);
        mputl(f1f,"foof.f");
        mputl(i,"sci_foof.c");

        mprintf("\n");
        mprintf(gettext("Calling ilib_for_link to build a Fortran subroutine.\n"));

        lib_ = ilib_for_link(["foof"], "foof.f", [],"f");
        link(lib_, "foof", "f");
        ilib_build("gw_foof",["foof" "sci_foof"],"sci_foof.c",basename(lib_));
        exec loader.sce ;
        chdir(path)
    end

    //Z = X+Y by fortran subroutine
    X = 5;
    Y = 7;

    mprintf("\n");
    mprintf(gettext("Calling Fortran subroutine. Z = X+Y"));
    mprintf("\n");
    mprintf(gettext("with X = %d"), X);
    mprintf("\n");
    mprintf(gettext("with Y = %d"), Y);
    mprintf("\n");
    mprintf("Z = foof(X, Y);");
    mprintf("\n");
    Z = foof(X, Y);
    mprintf(gettext("Result Z = %d"), Z);
    mprintf("\n");

end
