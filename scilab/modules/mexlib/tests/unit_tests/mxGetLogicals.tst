// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxGetLogicals mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    mxLogical *logicals = mxGetLogicals(prhs[0]);';
       '    plhs[0] = mxCreateLogicalScalar(logicals[0]);';
       '    plhs[1] = mxCreateLogicalScalar(logicals[1]);';
       '}'],'mexgetLogicals.c');
ilib_mex_build('libmextest',['getLogicals','mexgetLogicals','cmex'], 'mexgetLogicals.c',[],'Makelib','','','');
exec('loader.sce');

[out1, out2] = getLogicals([%t %f]);
if out1 <> %t then pause end
if out2 <> %f then pause end
