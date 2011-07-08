// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxGetNumberOfFields mex function
// ============================================================================

cd(TMPDIR);
mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    int num = mxGetNumberOfFields(prhs[0]);';
       '    plhs[0] = mxCreateDoubleScalar(num);';
       '}'],'mexgetNumberOfFields.c');
ilib_mex_build('libmextest',['getNumberOfFields','mexgetNumberOfFields','cmex'], 'mexgetNumberOfFields.c',[],'Makelib','','','');
exec('loader.sce');

c = struct("a", 5, "b", 3, "e", 1, "another key", "hello");
r = getNumberOfFields(c);
if r <> 4 then pause end

c = struct("a", 5, "b", 3);
r = getNumberOfFields(c);
if r <> 2 then pause end

c = struct("b", 32);
r = getNumberOfFields(c);
if r <> 1 then pause end

r = getNumberOfFields("not_a_struct");
if r <> 0 then pause end
