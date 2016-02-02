
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

mode(-1);
clear
D=pwd()
cd(TMPDIR)
mputl("ilib_mex_build(''libmex'',[''mexf16'',''mexfunction16'',''cmex''],[],[],'''','''','''','''')","builder.sce")

mputl([
"#include ""mex.h"""
"void mexFunction(nlhs, plhs, nrhs, prhs)"
"     int nlhs, nrhs;"
"     mxArray *plhs[]; mxArray *prhs[];"
"{"
"  int *dims = mxGetDimensions(prhs[0]);"
"  sciprint(""%d %d %d\n"",dims[0],dims[1],dims[2]);"
"}"
],"mexfunction16.c")
exec(TMPDIR+"/builder.sce");
exec(TMPDIR+"/loader.sce");
mexf16(rand(2,3,2))
cd(D)
