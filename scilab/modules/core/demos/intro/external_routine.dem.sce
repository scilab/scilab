//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//
mode(7)
//To quit this demo, enter CTRL+C, then type "abort"

if haveacompiler() then
    //           CALLING EXTERNAL ROUTINE

    f1=["void foo(double *a,double *b,double *c)";
    "{ *c = *a + *b; }"  ];
    
    i=["#include <stdlib.h>"
       "#include <api_scilab.h>"
       "#include <Scierror.h>"
       "#include <localization.h>"
       ""
       "extern int foo(double *x, double *y, double *z);"
       ""
       "int sci_foo(char *fname, void* pvApiCtx)"
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
       "  CheckInputArgument(pvApiCtx, 1, 2);"
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
       "  foo(pdVarOne, pdVarTwo, pdblOut);"
       ""
       "  AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;"
       "  ReturnArguments(pvApiCtx);"
       "  return 0;"
   "}"]

    // we use TMPDIR for compilation

    if ~c_link("foo") then
        path = pwd();
        chdir(TMPDIR);
        mputl(i, "sci_foo.c");
        mputl(f1,"foo.c");
        ilib_build(["gw_foo_c"],["foo", "sci_foo"],["foo.c", "sci_foo.c"],[]);
        exec loader.sce
        chdir(path)
    end

    //5+7 by C function
    foo(5,7)
end
