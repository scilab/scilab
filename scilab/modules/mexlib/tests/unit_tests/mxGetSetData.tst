// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxGetData and mxSetData mex functions
// ============================================================================

cd(TMPDIR);
mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    void *data = mxGetData(prhs[0]);';
       '    mxSetData(prhs[0], data);';
       '    plhs[0] = prhs[0];';
       '}'],'mexGetSetData.c');
ilib_mex_build('libmextest',['getSetData','mexGetSetData','cmex'], 'mexGetSetData.c',[],'Makelib','','','');
exec('loader.sce');

r = getSetData(double(1));
if r(1) <> 1 then pause end
r = getSetData(int8(2));
if r(1) <> 2 then pause end
r = getSetData(int16(3));
if r(1) <> 3 then pause end
r = getSetData(int32(4));
if r(1) <> 4 then pause end
r = getSetData(int64(5));
if r(1) <> 5 then pause end
r = getSetData(uint8(6));
if r(1) <> 6 then pause end
r = getSetData(uint16(7));
if r(1) <> 7 then pause end
r = getSetData(uint32(8));
if r(1) <> 8 then pause end
r = getSetData(uint64(9));
if r(1) <> 9 then pause end
r = getSetData(%t);
if r(1) <> %t then pause end