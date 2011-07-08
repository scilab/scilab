// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mexEvalString mex functions
// ============================================================================

cd(TMPDIR);
mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    char *string = mxArrayToString(prhs[0]);';
       '    plhs[0] = mxCreateDoubleScalar(mexEvalString(string));';
       '}'],'mexevalString.c');
ilib_mex_build('libmextest',['evalString','mexevalString','cmex'], 'mexevalString.c',[],'Makelib','','','');
exec('loader.sce');

r = evalString('printf ""mexEvalString works!\n"";');
if r <> 0 then pause end

global a;
r = evalString('a = 1;');
if a <> 1 then pause end
if r <> 0 then pause end

r = evalString('b =;');
if r <> 1 then pause end
