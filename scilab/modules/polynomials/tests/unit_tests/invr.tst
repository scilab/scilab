// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

assert_checkequal(invr([]), []);
assert_checkequal(invr(5), 0.2);
assert_checktrue(invr(5+0*%i)==0.2);
assert_checkequal(invr(5+0*%i), 0.2+0*%i);
assert_checkequal(invr(%s), 1/%s);
assert_checkequal(invr(%s^3), 1/%s^3);
assert_checkequal(invr(1/%s), %s+0/%s);
assert_checkequal(invr(1/%s^3), %s^3+0/%s);

p = [%s %s ; %s -%s];
assert_checkequal(p*invr(p), eye(2,2)+0/%s);
r = [1/%s 0 ; 0 1/%s^2];
assert_checkequal(invr(r), [%s 0 ; 0 %s^2]+0/%s);
