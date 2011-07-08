// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxCreateCellArray and mxCreateCellMatrix mex functions
// ============================================================================

cd(TMPDIR);
mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    mxArray *pOut1 = mxCreateCellMatrix(2, 2);';
       '    int dims[2] = {3, 3};';
       '    mxArray *pOut2 = mxCreateCellArray(2, dims);';
       '    plhs[0] = pOut1;';
       '    plhs[1] = pOut2;';
       '}'],'mexCreateCell.c');
ilib_mex_build('libmextest',['createCell','mexCreateCell','cmex'], 'mexCreateCell.c',[],'Makelib','','','');
exec('loader.sce');

[out1, out2] = createCell()
if size(out1) <> [2, 2] then pause end;
if size(out2) <> [3, 3] then pause end;
