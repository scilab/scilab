//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

n = 150;
xy = grand(n,2,"unf",0,2*%pi);
z = cos(xy(:,1)).*cos(xy(:,2));
xyz = [xy z];
tl_coef = cshep2d(xyz);
assert_checkequal(type(tl_coef), 16);
assert_checkequal(size(tl_coef), 8);
assert_checkequal(size(tl_coef(1)), [1 8]);
assert_checkequal(size(tl_coef(2)), [150 3]);
assert_checkequal(size(tl_coef(3)), [7 7]);
assert_checkequal(size(tl_coef(4)), [1 150]);
assert_checkequal(size(tl_coef(5)), [1 4]);
assert_checkequal(size(tl_coef(6)), [1 1]);
assert_checkequal(size(tl_coef(7)), [1 150]);
assert_checkequal(size(tl_coef(8)), [9 150]);
