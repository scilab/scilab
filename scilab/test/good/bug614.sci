// Non-regression test file for bug 614
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : 10 janvier 2006

mode(-1);
clear
D=pwd()
cd(TMPDIR)
mputl("ilib_mex_build(''libmex'',[''mexf16'',''mexfunction16'',''cmex''],[],[],''Makelib'','''','''','''')","builder.sce")

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
diary("bug614.dia")
mexf16(rand(2,3,2))
diary(0)
ok=mgetl("bug614.dia")=="2 3 2";
affich_result(ok,614);
cd(D)
