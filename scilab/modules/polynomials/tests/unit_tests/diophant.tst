// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->

// -------------------------
// Unit tests for diophant()
// -------------------------
// No solution
// -----------
s = %s;
[X, e] = diophant([0, 0], 1);
assert_checkequal(list(X, e), list([], %nan));

[X, e] = diophant([s^3,s^2], s);
assert_checkequal(list(X, e), list([], 1));

[X, e] = diophant(int8([2 0]), int8(1));  // No integer solution
assert_checkequal(list(X, e), list([], 1));


// p1 or p2 are zero : infinite number of solutions
// ------------------------------------------------
[X, e] = diophant([0, 0], 0);
assert_checkequal(list(X, e), list([1 1], -%inf));

[X, e] = diophant([0, 1], 2);
assert_checkequal(list(X, e), list([0 2], -%inf));

[X, e] = diophant([0, 2], 1);
assert_checkequal(list(X, e), list([0 0.5], -%inf));

[X, e] = diophant([1, 0], 2);
assert_checkequal(list(X, e), list([2 0], -%inf));

[X, e] = diophant([2, 0], 1);
assert_checkequal(list(X, e), list([0.5 0], -%inf));


// p1 <> 0 and p2 <> 0
// -------------------
[X, e] = diophant([4, 5], -3);
assert_checkequal(e, -%inf);
assert_checkalmostequal(X, [0 -0.6], %eps);

[X, e] = diophant([4, 5]+0*s, -3);
assert_checkequal(e, -%inf);
assert_checkalmostequal(X, [0 -0.6]+0*s, %eps);

[X, e] = diophant(int8([4, 7]), 5);
assert_checkequal(list(X, e), list(int8([10 -5]), 0));

[X, e] = diophant(int16([1234 5321]), 543);
assert_checkequal(list(X, e), list(int16([30533 -2339]), 0));

[X, e] = diophant([1+s ; 1-s^2], 1-s+s^2);
assert_checkequal(list(X, e), list([], 1.7320508075688774));

p = (1+s)*(s-1) + (1-s^2)*s;
[X, e] = diophant([1+s ; 1-s^2], -1+s+s^2-s^3);
assert_checkequal(list(X, e), list([-1+2*s-s^2;0], 0));

p = (1+s)*(s-1+s^2) - (1-s^2)*s^2;
[X, e] = diophant([1+s ; 1-s^2], -1+s^2+s^3+s^4);
assert_checkequal(list(X, e), list([-1+s+s^3;0], 0));

P = [3+s ; 7-s+4*s^2-s^3];
[X, e] = diophant(P, s^2);
assert_checkequal(e, 0);
assert_checkequal(clean(X.' * P), s^2);

// 5 general random successfull cases
for i = 1:5
    d = grand(1,4,"uin",0,5); // degrees
    p1 = 0;
    while p1==0
        p1 = poly(grand(1,d(1)+1,"uin",-9,9),"s","coeff");
    end
    p2 = 0;
    while p2==0
        p2 = poly(grand(1,d(2)+1,"uin",-9,9),"s","coeff");
    end
    x1 = poly(grand(1,d(3)+1,"uin",-9,9),"s","coeff");
    x2 = poly(grand(1,d(4)+1,"uin",-9,9),"s","coeff");
    p = p1*x1 + p2*x2;
    [X, e] = diophant([p1 p2], p);
    assert_checkequal(e, 0);
    assert_checkalmostequal(clean(X * [p1 ; p2]), p, 1e-12);
end

// Output format
// -------------
X = diophant([0, 0], 0)
assert_checkequal(size(X), [1 2]);
X = diophant([0; 0], 0)
assert_checkequal(size(X), [2 1]);
X = diophant([0, 1], 2);
assert_checkequal(size(X), [1 2]);
X = diophant([0; 1], 2);
assert_checkequal(size(X), [2 1]);

// Output type
// -----------
[X, e] = diophant([0, 0], 0)
assert_checkequal(type(X), 1);
[X, e] = diophant([1, 1], 1)
assert_checkequal(type(X), 1);
[X, e] = diophant([1, 1], int8(1))
assert_checkequal(type(X), 1);
[X, e] = diophant([int8(0), int8(0)], 0)
assert_checkequal(typeof(X), "int8");
[X, e] = diophant([int8(1), int8(1)], 1)
assert_checkequal(typeof(X), "int8");

// Errors
// ------
cmd = "diophant([%nan, 1], 2)";
msg = "diophant: Argument #1: Inf or NaN coefficients not supported.";
assert_checkerror(cmd, msg);

cmd = "diophant([2, 1], %nan)";
msg = "diophant: Argument #2: Inf or NaN coefficients not supported.";
assert_checkerror(cmd, msg);
