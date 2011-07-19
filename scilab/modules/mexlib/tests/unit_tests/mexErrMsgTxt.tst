// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// <-- INTERACTIVE TEST -->
// ============================================================================
// Unitary tests for mexErrMsgTxt mex function
// ============================================================================

cd(TMPDIR);

ilib_verbose(0);

mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    mexErrMsgTxt(""An useless error message!"");';
       '}'],'mexerrMsgTxt.c');
ilib_mex_build('libmextest',['errMsgTxt','mexerrMsgTxt','cmex'], 'mexerrMsgTxt.c',[],'Makelib','','','');
exec('loader.sce');

errMsgTxt()
