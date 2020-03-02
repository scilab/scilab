// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON - Le Mans Université
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

//
// <-- Short Description -->
// Unit tests for setdiff() with dense input booleans
// Sparse booleans are tested in setdiff_sparse.tst
//
// See also:
//   Bug 16451 : http://bugzilla.scilab.org/16451
//               setdiff(a, b, "r"|"c") yields an error when a & b are boolean
// --------------------------------------------------------------------------

// ===========
// By elements
// ===========
// No k requested
// --------------
// b = []
assert_checkequal(setdiff([], []), []);
assert_checkequal(setdiff(%t, []), %t);
assert_checkequal(setdiff(%f, []), %f);
assert_checkequal(setdiff([%f %t], []), [%f %t]);
assert_checkequal(setdiff([%t %f], []), [%f %t]);
assert_checkequal(setdiff([%t %f %t], []), [%f %t]);

assert_checkequal(setdiff([%f %t]', []), [%f %t]');
assert_checkequal(setdiff([%t %f]', []), [%f %t]');
assert_checkequal(setdiff([%t %f %t]', []), [%f %t]');

assert_checkequal(setdiff([%f %t ; %f %f], []), [%f %t]');

// b <> []
assert_checkequal(setdiff([], %f), []);
assert_checkequal(setdiff([], [%t %f]), []);
assert_checkequal(setdiff(%t, %f), %t);
assert_checkequal(setdiff(%f, %t), %f);
assert_checkequal(setdiff(%f, [%t %f]), []);
assert_checkequal(setdiff([%f %t], [%t %f]), []);
assert_checkequal(setdiff([%f %t], %t), %f);
assert_checkequal(setdiff([%f %t], %f), %t);

// k requested
// -----------
[d, k] = setdiff([], []);
assert_checkequal(d, []);
[d, k] = setdiff(%t, []);
assert_checkequal(d, %t);
[d, k] = setdiff(%f, []);
assert_checkequal(d, %f);
[d, k] = setdiff([%f %t], []);
assert_checkequal(d, [%f %t]);
[d, k] = setdiff([%t %f], []);
assert_checkequal(d, [%f %t]);
[d, k] = setdiff([%t %f %t], []);
assert_checkequal(d, [%f %t]);

[d, k] = setdiff([%f %t]', []);
assert_checkequal(d, [%f %t]');
[d, k] = setdiff([%t %f]', []);
assert_checkequal(d, [%f %t]');
[d, k] = setdiff([%t %f %t]', []);
assert_checkequal(d, [%f %t]');

[d, k] = setdiff([%f %t ; %f %f], []);
assert_checkequal(d, [%f %t]');

// b <> []
[d, k] = setdiff([], %f);
assert_checkequal(d, []);
assert_checkequal(k, []);
[d, k] = setdiff([], [%t %f]);
assert_checkequal(d, []);
assert_checkequal(k, []);
[d, k] = setdiff(%t, %f);
assert_checkequal(d, %t);
assert_checkequal(k, 1);
[d, k] = setdiff(%f, %t);
assert_checkequal(d, %f);
assert_checkequal(k, 1);
[d, k] = setdiff(%f, [%t %f]);
assert_checkequal(d, []);
assert_checkequal(k, []);
[d, k] = setdiff([%f %t], [%t %f]);
assert_checkequal(d, []);
assert_checkequal(k, []);
[d, k] = setdiff([%f %t], %t);
assert_checkequal(d, %f);
assert_checkequal(k, 1);
[d, k] = setdiff([%f %t], %f);
assert_checkequal(d, %t);
assert_checkequal(k, 2);


// ==========
// "r" or "c"
// ==========
a  = [
  %T %F %F %T %F %T %F
  %T %T %T %T %T %F %T
  %F %F %T %T %F %T %T
  ];
b = [
  %F %T %F %T %T %T %F
  %T %F %F %T %T %F %T
  %T %T %T %T %T %F %T
 ];
// No ka requested
// ---------------
d = setdiff(a, [], "c");
assert_checkequal(d, unique(a,"c"));
d = setdiff(a, [], "r");
assert_checkequal(d, unique(a,"r"));
d = setdiff(a, b, "c");
assert_checkequal(d, [0 1 0 ; 1 1 0]'==1);
d = setdiff(a', b', "r");
assert_checkequal(d, [0 1 0 ; 1 1 0]==1);
d = setdiff(a, b, "r");
assert_checkequal(d, [0 0 1 1 0 1 1 ; 1 0 0 1 0 1 0]==1);

// ka requested
// ------------
[d, k] = setdiff(a, [], "c");
assert_checkequal(d, unique(a,"c"));
assert_checkequal(k, [2 3 6 1 4]);
[d, k] = setdiff(a, [], "r");
assert_checkequal(d, unique(a,"r"));
assert_checkequal(k, [3 1 2]');

[d, k] = setdiff(a, b, "c");
assert_checkequal(d, [0 1 0 ; 1 1 0]'==1);
assert_checkequal(k, [2 1]);
[d, k] = setdiff(a', b', "r");
assert_checkequal(d, [0 1 0 ; 1 1 0]==1);
assert_checkequal(k, [2 1]');
[d, k] = setdiff(a, b, "r");
assert_checkequal(d, [0 0 1 1 0 1 1 ; 1 0 0 1 0 1 0]==1);
assert_checkequal(k, [3 1]');

// ===================
// Boolean hypermatrix
// ===================
T = %T; F = %F;
b  = [
  F F T F F T T T T F
  T T F F T F F T T F
  T F F F F F F T F T
  T F T F T F F F F T ];
h = cat(3 , [
  T F T T
  F F F F
  F T F T
  F T T F ], [
  T F F F
  F F F F
  F T F T
  T F F F ]);

// By elements
// -----------
assert_checkequal(setdiff(h, b), []);
assert_checkequal(setdiff(b, h), []);

// With "r" and "c"
// ----------------
ref = [
  F T
  F F
  T T
  F F ];
assert_checkequal(setdiff(h, b, "c"), ref);
assert_checkequal(setdiff(cat(3, h(:,:,1)', h(:,:,2)'), b', "r"), ref');
ref = [
  F F F T T
  T T T T T
  F F T F T
  F T T F F ];
assert_checkequal(setdiff(b, h, "c"), ref);
