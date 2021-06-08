// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// computes the integrand exp(x*x+y*y+z*z) over the
//tetrahedron (0.,0.,0.),(1.,0.,0.),(0.,1.,0.),(0.,0.,1.)

X=[0;1;0;0];
Y=[0;0;1;0];
Z=[0;0;0;1];
[RESULT,ERROR]=int3d(X,Y,Z,'int3dex');

assert_checkalmostequal(RESULT,0.22779999698985758938);
assert_checktrue(ERROR < 2e-6);

//integration over a cube  -1<=x<=1;-1<=y<=1;-1<=z<=1

function v=f1(xyz,numfun),v=exp(xyz'*xyz),endfunction
[result,err]=int3d(-1,1,-1,1,-1,1,f1,1,[0,100000,1.d-5,1.d-7]);

assert_checkalmostequal(result,25.0329944788486784546);
assert_checktrue(err < 1e-5);

function v=f2(xyz,numfun),v=1,endfunction
[result,err]=int3d(-1,1,-1,1,-1,1,f2,1,[0,100000,1.d-5,1.d-7]);

assert_checkalmostequal(result,8);
assert_checktrue(err < 9e-14);
