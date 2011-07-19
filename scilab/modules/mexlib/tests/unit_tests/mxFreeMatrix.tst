// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxFreeMatrix mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    mxArray *d = prhs[0];'
       '    mxFreeMatrix(d);';
       '    if (*d == NULL)';
       '    {';
       '        mexPrintf(""It works."");';
       '    }';
       '}'],'mexfreeMatrix.c');

ilib_mex_build('libmextest',['freeMatrix','mexfreeMatrix','cmex'], 'mexfreeMatrix.c',[],'Makelib','','','');
exec('loader.sce');

freeMatrix(cell());
freeMatrix('s');
freeMatrix(double(1));
freeMatrix(int8(1));
freeMatrix(int16(1));
freeMatrix(int32(1));
freeMatrix(int64(1));
freeMatrix(%t);
freeMatrix(struct());
freeMatrix(uint8(1));
freeMatrix(uint16(1));
freeMatrix(uint32(1));
freeMatrix(uint64(1));
