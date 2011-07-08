// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxIsClass mex function
// ============================================================================

cd(TMPDIR);

mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    if (mxIsClass(prhs[0], ""cell"")) {';
       '        mexPrintf(""cell"");';
       '    }'
       '    if (mxIsClass(prhs[0], ""char"")) {';
       '        mexPrintf(""char"");';
       '    }'
       '    if (mxIsClass(prhs[0], ""double"")) {';
       '        mexPrintf(""double"");';
       '    }'
       '    if (mxIsClass(prhs[0], ""function_handle"")) {';
       '        mexPrintf(""function_handle"");';
       '    }'
       '    if (mxIsClass(prhs[0], ""int8"")) {';
       '        mexPrintf(""int8"");';
       '    }'
       '    if (mxIsClass(prhs[0], ""int16"")) {';
       '        mexPrintf(""int16"");';
       '    }'
       '    if (mxIsClass(prhs[0], ""int32"")) {';
       '        mexPrintf(""int32"");';
       '    }'
       '    if (mxIsClass(prhs[0], ""int64"")) {';
       '        mexPrintf(""int64"");';
       '    }'
       '    if (mxIsClass(prhs[0], ""logical"")) {';
       '        mexPrintf(""logical"");';
       '    }'
       '    if (mxIsClass(prhs[0], ""single"")) {';
       '        mexPrintf(""single"");';
       '    }'
       '    if (mxIsClass(prhs[0], ""struct"")) {';
       '        mexPrintf(""struct"");';
       '    }'
       '    if (mxIsClass(prhs[0], ""uint8"")) {';
       '        mexPrintf(""uint8"");';
       '    }'
       '    if (mxIsClass(prhs[0], ""uint16"")) {';
       '        mexPrintf(""uint16"");';
       '    }'
       '    if (mxIsClass(prhs[0], ""uint32"")) {';
       '        mexPrintf(""uint32"");';
       '    }'
       '    if (mxIsClass(prhs[0], ""uint64"")) {';
       '        mexPrintf(""uint64"");';
       '    }'
       '}'],'mexisClass.c');

ilib_mex_build('libmextest',['printClass','mexisClass','cmex'], 'mexisClass.c',[],'Makelib','','','');
exec('loader.sce');

printClass(cell());
printClass('s');
printClass(1);
printClass(sum);
printClass(int8(1));
printClass(int16(1));
printClass(int32(1));
printClass(int64(1));
printClass(%t);
printClass(struct());
printClass(uint8(1));
printClass(uint16(1));
printClass(uint32(1));
printClass(uint64(1));
