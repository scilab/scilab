mode(-1);
clear
D=pwd()
cd(TMPDIR)
mputl('ilib_mex_build(''libmex'',[''mexf16'',''mexfunction16'',''cmex''],[],[],''Makelib'','''','''','''')','builder.sce')

mputl([
'#include ""mex.h""'
'void mexFunction(nlhs, plhs, nrhs, prhs)'
'     int nlhs, nrhs;'
'     mxArray *plhs[]; mxArray *prhs[];'
'{'
'  int *dims = mxGetDimensions(prhs[0]);'
'  sciprint(""%d %d %d\n"",dims[0],dims[1],dims[2]);'
'}'
],'mexfunction16.c')
exec(TMPDIR+'/builder.sce');
exec(TMPDIR+'/loader.sce');
mexf16(rand(2,3,2))
cd(D)
