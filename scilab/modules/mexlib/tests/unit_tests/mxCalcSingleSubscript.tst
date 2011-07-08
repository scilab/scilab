// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxCalcSingleSubscript mex function
// ============================================================================

cd(TMPDIR);
mputl([ '#include ""mex.h""';
        'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
        '{';
        '    int *dims = mxGetData(prhs[2]);';
        '    int ndim = mxGetScalar(prhs[1]);';
        '    plhs[0] = mxCreateDoubleScalar(mxCalcSingleSubscript(prhs[0], ndim, dims));';
        '}'],'mexcalcSingleSubscript.c');
ilib_mex_build('libmextest', ['calcSingleSubscript', 'mexcalcSingleSubscript', 'cmex'], 'mexcalcSingleSubscript.c', [], 'Makelib', '', '', '');
exec('loader.sce');

a = zeros(3, 3);
r = calcSingleSubscript(a, 2, int32([0 0]));
if r <> 0 then pause end
r = calcSingleSubscript(a, 2, int32([1 1]));
if r <> 4 then pause end
r = calcSingleSubscript(a, 2, int32([2 2]));
if r <> 8 then pause end

b = zeros(4, 2, 3);
r = calcSingleSubscript(b, 3, int32([0 0 0]));
if r <> 0 then pause end
r = calcSingleSubscript(b, 2, int32([0 0]));
if r <> 0 then pause end
r = calcSingleSubscript(b, 3, int32([3 0 2]));
if r <> 19 then pause end
r = calcSingleSubscript(b, 3, int32([1 1 1]));
if r <> 13 then pause end
r = calcSingleSubscript(b, 3, int32([3 1 2]));
if r <> 23 then pause end
