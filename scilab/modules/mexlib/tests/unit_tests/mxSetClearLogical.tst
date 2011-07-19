// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// <-- NOT FIXED -->
// ============================================================================
// Unitary tests for mxSetLogical and mxClearLogical mex functions
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    mxArray *pOut1 = prhs[0];'
       '    mxArray *pOut2 = prhs[1];'
       '    mxArray *pOut3 = prhs[2];'
       '    mxSetLogical(pOut1);';
       '    mxSetLogical(pOut2);';
       '    mxClearLogical(pOut3);';
       '    plhs[0] = pOut1;';
       '    plhs[1] = pOut2;';
       '    plhs[2] = pOut3;';
       '}'],'mexsetClearLogical.c');
ilib_mex_build('libmextest',['setClearLogical','mexsetClearLogical','cmex'], 'mexsetClearLogical.c',[],'Makelib','','','');
exec('loader.sce');

[out1, out2, out3] = setClearLogical([1 0], int32([1 0]), [%t %f]);
if out1 <> [%t %f] then pause end
if out2 <> [%t %f] then pause end
if out3 <> [1 0] then pause end
