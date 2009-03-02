// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 614 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=614
//
// <-- Short Description -->
//    mxGetDimensions() from mex.h and mexlib.c do not work 
//    properly. The reason is that it returns a pointer to int 
//    like Matlab, but in fact, the dimensions in scilab are in 
//    double format.
//
//    Also occur in other OS's.

ilib_verbose(0);

my_current_directory = pwd();
cd(TMPDIR);
mputl('ilib_mex_build(''libmex'',[''mexf16'',''mexfunction16'',''cmex''],[''mexfunction16.c''],[],''Makelib'','''','''','''')','builder.sce');

mputl([
'#include ""mex.h""'
'void mexFunction(nlhs, plhs, nrhs, prhs)'
'     int nlhs, nrhs;'
'     mxArray *plhs[]; mxArray *prhs[];'
'{'
'  int *dims = mxGetDimensions(prhs[0]);'
'  sciprint(""%d %d %d\n"",dims[0],dims[1],dims[2]);'
'}'
],'mexfunction16.c');

exec('builder.sce');
exec('loader.sce');

mexf16(rand(2,3,2))

cd(my_current_directory);
