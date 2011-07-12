// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxGetNumberOfElements mex function
// ============================================================================

cd(TMPDIR);
mputl([ '#include ""mex.h""';
        'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
        '{';
        '   int iSize = 0;';
        '   mxArray* pOut = NULL;';
        '';
        '   if(nrhs != 1)';
        '   {';
        '       mexErrMsgTxt(""Wrong number of input argument"");';
        '   }';
        '';
        '   iSize = mxGetNumberOfElements(prhs[0]);';
        '   pOut = mxCreateDoubleScalar((double)iSize);';
        '   plhs[0] = pOut;';
        '}'],'mexGetNumberOfElements.c');
ilib_mex_build('libmextest',['getNumberOfElements','mexGetNumberOfElements','cmex'], 'mexGetNumberOfElements.c',[],'Makelib','','','');
exec('loader.sce');

a = ones(1);
var = getNumberOfElements(a);
if var <> size(a, "*") then pause end

a = ones(1, 2);
var = getNumberOfElements(a);
if var <> size(a, "*") then pause end

a = ones(1, 2, 3);
var = getNumberOfElements(a);
if var <> size(a, "*") then pause end

a = ones(1, 2, 3, 4);
var = getNumberOfElements(a);
if var <> size(a, "*") then pause end

