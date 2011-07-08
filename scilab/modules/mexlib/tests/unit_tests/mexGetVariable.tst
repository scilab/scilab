// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mexGetVariable mex function
// ============================================================================

cd(TMPDIR);
mputl([ '#include ""mex.h""';
        'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
        '{';
        '    char *workspace = mxArrayToString(prhs[0]);';
        '    char *name = mxArrayToString(prhs[1]);';
        '    mxArray *pOut = mexGetVariable(workspace, name);';
        '    plhs[0] = pOut != NULL ? pOut : mxCreateString(""NULL"");';
        '}'],'mexgetVariable.c');
ilib_mex_build('libmextest', ['getVariable', 'mexgetVariable', 'cmex'], 'mexgetVariable.c', [], 'Makelib', '', '', '');
exec('loader.sce');

global globalvar;
globalvar = "my global var";
callervar = "my caller var";
if getVariable("global", "globalvar") <> "my global var" then pause end
if getVariable("caller", "globalvar") <> "NULL" then pause end
if getVariable("base", "globalvar") <> "my global var" then pause end
if getVariable("global", "callervar") <> "NULL" then pause end
if getVariable("caller", "callervar") <> "my caller var" then pause end
if getVariable("base", "callervar") <> "my caller var" then pause end
