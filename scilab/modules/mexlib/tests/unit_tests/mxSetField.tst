// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxSetField mex function
// ============================================================================

cd(TMPDIR);
mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    mxSetField(prhs[0], 0, ""a"", mxCreateDoubleScalar(3));';
       '    plhs[0] = prhs[0];';
       '}'],'mexsetField.c');
ilib_mex_build('libmextest',['setField','mexsetField','cmex'], 'mexsetField.c',[],'Makelib','','','');
exec('loader.sce');


s = struct("a", 1, "b", 2);
r = setField(s);
if r("a") <> 3 then pause end
