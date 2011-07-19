// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxMalloc mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    double *data = mxMalloc(2 * sizeof(double *));';
       '    data[0] = 1.0;';
       '    data[1] = 3.2;';
       '    plhs[0] = mxCreateDoubleMatrix(2, 1, mxREAL);'
       '    mxSetPr(plhs[0], data);';
       '}'],'mexmalloc.c');
ilib_mex_build('libmextest',['malloc','mexmalloc','cmex'], 'mexmalloc.c',[],'Makelib','','','');
exec('loader.sce');

r = malloc();
if r(1) <> 1 then pause end
if r(2) <> 3.2 then pause end
