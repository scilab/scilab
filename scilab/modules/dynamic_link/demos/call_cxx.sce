//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI Group - Clement DAVID
//
// This file is distributed under the same license as the Scilab package.
//

//           CALLING EXTERNAL C FUNCTION

if haveacompiler() then


    // we use TMPDIR for compilation

    f1=["extern ""C"""
    "void fooxx(double *a,double *b,double *c)";
    "{ *c = *a + *b; }"  ];

    i=["#include <stdlib.h>"
    "#include <api_scilab.h>"
    "#include <Scierror.h>"
    "#include <localization.h>"
    ""
    "extern int fooxx(double *x, double *y, double *z);"
    ""
    "int sci_fooxx(char *fname, void* pvApiCtx)"
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
    "  fooxx(pdVarOne, pdVarTwo, pdblOut);"
    ""
    "  AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;"
    "  ReturnArguments(pvApiCtx);"
    "  return 0;"
    "}"]

    mprintf("\n");
    mprintf(gettext("Calling a C++ function from Scilab.\n"));

    disp(f1);

    if ~c_link("fooxx") then
        cur_ilib_verbose = ilib_verbose();
        ilib_verbose(1);
        path = pwd();
        chdir(TMPDIR);
        mputl(f1,"fooxx.cxx");
        mputl(i,"sci_fooxx.c");

        mprintf("\n");
        mprintf(gettext("Calling ilib_for_link to build C++ function.\n"));
        lib_ = ilib_for_link(["fooxx"], "fooxx.cxx", [],"c");
        link(lib_, "fooxx", "c");
        ilib_build("gw_fooxx",["fooxx" "sci_fooxx"],"sci_fooxx.c",basename(lib_));
        exec loader.sce ;
        chdir(path);
        ilib_verbose(cur_ilib_verbose);
    end

    //Z = X+Y by C function
    X = 5;
    Y = 7;
    mprintf("\n");
    mprintf(gettext("Calling C++ function. Z = X+Y"));
    mprintf("\n");
    mprintf(gettext("with X = %d"), X);
    mprintf("\n");
    mprintf(gettext("with Y = %d"), Y);
    mprintf("\n");
    mprintf("Z = fooxx(X, Y);");
    mprintf("\n");
    Z = fooxx(X, Y);
    mprintf(gettext("Result Z = %d"), Z);
    mprintf("\n");

end
