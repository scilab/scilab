// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 10078 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10078
//
// <-- Short Description -->
// isinf() was not reliable for polynomials

p = poly([1 %inf],"s","coeff");
assert_checktrue(isinf(p));
p = poly([1 %inf %i],"s","coeff");
assert_checktrue(isinf(p));
p = poly([1 %nan %inf],"x","coeff");
assert_checktrue(isinf(p));
p = poly([1 %nan %inf %i],"x","coeff");
assert_checktrue(isinf(p));

assert_checkequal(isinf(poly(%inf,"x","coeff")), %t);
assert_checkequal(isinf([p p]), [%t %t]);
assert_checkequal(isinf([p ; p]), [%t ; %t]);
assert_checkequal(isinf(cat(3,p,p)), cat(3,%t,%t));
