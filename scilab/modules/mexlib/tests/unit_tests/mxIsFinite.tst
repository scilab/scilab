// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxIsFinite mex function
// ============================================================================

cd(TMPDIR);
mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    double value = mxGetScalar(prhs[0]);';
       '    bool isFinite = mxIsFinite(value);';
       '    mxArray* pOut = mxCreateLogicalScalar(isFinite);';
       '    plhs[0] = pOut;';
       '}'],'mexisFinite.c');
ilib_mex_build('libmextest',['isFinite','mexisFinite','cmex'], 'mexisFinite.c',[],'Makelib','','','');
exec('loader.sce');

a = isFinite(%inf);
if a <> %f then pause end
a = isFinite(1/0);
if a <> %f then pause end
a = isFinite(100);
if a <> %t then pause end
a = isFinite(0);
if a <> %t then pause end
a = isFinite(%nan);
if a <> %f then pause end
