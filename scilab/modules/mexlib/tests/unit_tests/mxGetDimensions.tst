// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxGetDimensions mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    int ndim = mxGetNumberOfDimensions(prhs[0]);';
       '    int *dims = mxGetDimensions(prhs[0]);';
       '    mxArray *ptr = mxCreateNumericMatrix(1, ndim, mxINT32_CLASS, mxREAL);';
       '    mxSetPr(ptr, dims);';
       '    plhs[0] = ptr;';
       '}'],'mexgetDimensions.c');
ilib_mex_build('libmextest',['getDimensions','mexgetDimensions','cmex'], 'mexgetDimensions.c',[],'Makelib','','','');
exec('loader.sce');

if getDimensions([1]) <> [1 1] then pause end
if getDimensions([1 1 1]) <> [1 3] then pause end
if getDimensions([1;1;1]) <> [3 1] then pause end
if getDimensions([1 1;2 2;3 3]) <> [3 2] then pause end
if getDimensions(zeros(4, 2, 3)) <> [4 2 3] then pause end
