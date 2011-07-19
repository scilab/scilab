// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxGetString mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    char string[20];';
       '    plhs[0] = mxCreateDoubleScalar(mxGetString(prhs[0], string, 20));';
       '    plhs[1] = mxCreateString(string);';
       '}'],'mexgetString.c');
ilib_mex_build('libmextest',['getString','mexgetString','cmex'], 'mexgetString.c',[],'Makelib','','','');
exec('loader.sce');

[out1, out2] = getString("String to copy.");
if out1 <> 0 then pause end
if out2 <> "String to copy." then pause end

[out3, out4] = getString("A string bigger than 20 chars.");
if out3 <> 1 then pause end
if out4 <> "A string bigger tha" then pause end

[out5, out6] = getString(["multiline"; "string"]);
if out5 <> 0 then pause end
if out6 <> "multilinestring" then pause end
