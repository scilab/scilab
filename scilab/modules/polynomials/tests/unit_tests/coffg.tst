// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// With []
// -------
[n, d] = coffg([]);
assert_checkequal(n, []);
assert_checkequal(d, []);

// With scalars
// ------------
[n, d] = coffg(5);
assert_checkequal(n, 1);
assert_checkequal(d, 5);

[n, d] = coffg(5+0*%i);
assert_checkequal(n, 1 + 0*%i);
assert_checkequal(d, 5 + 0*%i);

[n, d] = coffg(%s);
assert_checkequal(n, 1 + 0/%s);
assert_checkequal(d, %s);

[n, d] = coffg(1+%s^2);
assert_checkequal(n, 1 + 0/%s);
assert_checkequal(d, 1 + %s^2);

[n, d] = coffg(1/(1+%s^2));
assert_checkequal(n, 1 + 0/%s);
assert_checkequal(d, 1/(1+%s^2));


// With matrices
// -------------
p = [%s %s ; %s -%s];
[n, d] = coffg(p);
assert_checkequal(n, -p);
assert_checkequal(d, -2*%s^2);
assert_checkequal(p*n/d-eye(), zeros(2,2)/%s);

r = [1/%s 0 ; 0 %s^2];
[n, d] = coffg(r);
assert_checkequal(n, [%s^2 0 ; 0 1/%s]);
assert_checkequal(d, %s^2/%s);
assert_checkequal(r*n/d - eye(), zeros(2,2)/%s);
