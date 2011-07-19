// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mexPutVariable mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ '#include ""mex.h""';
        'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
        '{';
        '    char *workspace = mxArrayToString(prhs[0]);';
        '    char *name = mxArrayToString(prhs[1]);';
        '    mxArray *pOut = mexPutVariable(workspace, name, prhs[2]);';
        '}'],'mexputVariable.c');
ilib_mex_build('libmextest', ['putVariable', 'mexputVariable', 'cmex'], 'mexputVariable.c', [], 'Makelib', '', '', '');
exec('loader.sce');

global globalvar;
putVariable("global", "globalvar", 32);
if globalvar <> 32 then pause end
if isglobal('globalvar') <> %t then pause end

othervar = 1;
putVariable("base", "othervar", 3);
if othervar <> 1 then pause end
if isglobal('othervar') <> %f then pause end

putVariable("caller", "othervar", 3);
if othervar <> 3 then pause end
if isglobal('othervar') <> %f then pause end

putVariable("caller", "caller_defined_in_C", 0);
if exists('caller_defined_in_C') <> 1 then pause end
