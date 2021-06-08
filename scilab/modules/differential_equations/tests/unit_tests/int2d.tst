// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

X=[0,0;1,1;1,0];
Y=[0,0;0,1;1,1];
function z=f(x,y)
    z=cos(x+y)
endfunction

[I,e]=int2d(X,Y,f);
assert_checkalmostequal(I,4*sin(.5)^2*cos(1))
assert_checktrue(e<4e-11)

[I,e]=int2d(0,1,0,1,f);
assert_checkalmostequal(I,4*sin(.5)^2*cos(1))
assert_checktrue(e<7e-14)
