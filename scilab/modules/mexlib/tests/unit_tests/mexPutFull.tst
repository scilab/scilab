// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mexPutFull mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ '#include ""mex.h""';
        'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
        '{';
        '    double pr[2] = {2, 5};';
        '    double pi[2] = {3, 0};';
        '    mexPutFull(""var1"", 2, 1, pr, NULL);';
        '    plhs[0] = mexGetVariable(""caller"", ""var1"");';
        '    mexPutFull(""var2"", 1, 2, pr, pi);';
        '    plhs[1] = mexGetVariable(""caller"", ""var2"");';
        '}'],'mexputFull.c');
ilib_mex_build('libmextest', ['putFull', 'mexputFull', 'cmex'], 'mexputFull.c', [], 'Makelib', '', '', '');
exec('loader.sce');

[out1, out2] = putFull();
if exists("var1") <> 1 then pause end
if exists("var2") <> 1 then pause end
if out1(1) <> 2 then pause end
if out1(2) <> 5 then pause end
if out2(1) <> complex(2, 3) then pause end
if out2(2) <> 5 then pause end