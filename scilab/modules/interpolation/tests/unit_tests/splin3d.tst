//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


function v=f(x,y,z)
    v=cos(2*%pi*x).*sin(2*%pi*y).*cos(2*%pi*z)
endfunction

// n x n x n  interpolation points
n = 10;
// interpolation grid
x = linspace(0,1,n); y=x; z=x;
[X,Y,Z] = ndgrid(x,y,z);
V = f(X,Y,Z);
tl = splin3d(x,y,z,V,[5 5 5]);
assert_checkequal(type(tl), 16);
assert_checkequal(size(tl), 7);
assert_checkequal(type(tl(1)), 10);
assert_checkequal(size(tl(2)), [15 1]);
assert_checkequal(size(tl(3)), [15 1]);
assert_checkequal(size(tl(4)), [15 1]);
assert_checkequal(size(tl(6)), [1000 1]);
assert_checkequal(size(tl(7)), [6 1]);
