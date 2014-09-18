// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- unit tests for function filter  -->
//
// <-- CLI SHELL MODE -->

//==============================================================================
// Error handling tests
//==============================================================================

fname = "filter";
func_call = "filter(B, A, x)";

// Type checks
err_msg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix or polynomial expected.\n"), fname, 1);
B = "test";
A = [0 1];
x = [1, zeros(1,99)];
assert_checkerror(func_call, err_msg);

err_msg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix or polynomial expected.\n"), fname, 2);
B = [0 0 1];
A = "test";
assert_checkerror(func_call, err_msg);

err_msg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 3);
B = [0 0 1];
A = [1 0 1];
x = "test";
assert_checkerror(func_call, err_msg);

err_msg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 4);
x = [1, zeros(1,99)];
z = "test";
func_call = "filter(B, A, x, z)";
assert_checkerror(func_call, err_msg);

// Values are real
err_msg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix or polynomial expected.\n"), fname, 1);
z = 0;
B = %i;
assert_checkerror(func_call, err_msg);

err_msg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix or polynomial expected.\n"), fname, 2);
A = 2 * %i;
B = [0 0 1];
assert_checkerror(func_call, err_msg);

err_msg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 3);
A = [1 0 1];
x = [%i 1 3 4];
assert_checkerror(func_call, err_msg);

x = [1 zeros(1, 99)];
z = %i;
err_msg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 4);
assert_checkerror(func_call, err_msg);

// Check vector values
A = [A; A];
z = 0;
err_msg = msprintf(_("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 1);
assert_checkerror(func_call, err_msg);

A = A(1, :);
B = [B; B];
err_msg = msprintf(_("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 2);
assert_checkerror(func_call, err_msg);

B = B(1, :);
x = [x; x];
err_msg = msprintf(_("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 3);
assert_checkerror(func_call, err_msg);

x = x(1, :);
z = [1 2; 3 4];
err_msg = msprintf(_("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 4);
assert_checkerror(func_call, err_msg);

// User mixes polynomial and vector notation
z = 0;
B = %s^3 + %s + 1;
A = [1 0];
err_msg = msprintf(_("%s: Incompatible input arguments #%d and #%d: a polynomial and 1-by-1 matrix or two polynomials expected.\n"), fname, 1, 2);
assert_checkerror(func_call, err_msg);

A = B;
B = [1 1];
err_msg = msprintf(_("%s: Incompatible input arguments #%d and #%d: a polynomial and 1-by-1 matrix or two polynomials expected.\n"), fname, 1, 2);
assert_checkerror(func_call, err_msg);

// Denominator must have first coefficient not equal to 0
A = [0 0 1];
err_msg = msprintf(_("%s: Wrong value for input argument #%d: First element must not be %s.\n"), fname, 2, "0");
assert_checkerror(func_call, err_msg);

B = %s^3 + %s + 1;
A = %s;
err_msg = msprintf(_("%s: Wrong value for input argument #%d: First element must not be %s.\n"), fname, 2, "0");
assert_checkerror(func_call, err_msg);

//==============================================================================
// Nominal behaviour
//==============================================================================

// Integrator y(n) = y(n - 1) + x(n)
// Filter is 1/(1 - 1 * z**-1)
B = 1;
A = [1 -1];
x = [1 1 1 1 1];

y = [1 2 3 4 5];
res = filter(B, A, x);
assert_checkalmostequal(res, y);

// Same behaviour with a polynomial
B = %s;
A = %s - 1;
res = filter(B, A, x);
assert_checkalmostequal(res, y);

// Integrator with delay y(n) = y(n-1) + x(n-1)
// Filter is z**-1 / (1 + z**-1)
B = 1;
y = [0 1 2 3 4];
res = filter(B, A, x);
assert_checkalmostequal(res, y);

B = [0 1];
A = [1 -1];
res = filter(B, A, x);
assert_checkalmostequal(res, y);

// Derivator y(n) = x(n) - x(n-1)
// Filter is 1 - z**-1
B = [1 -1];
A = 1;
x = [1 3 5 3 1];
y = [1 2 2 -2 -2];
res = filter(B, A, x);
assert_checkalmostequal(res, y);

// Complex filter y(n) = 2 * y(n-1) - 3 * y(n-2) + x(n-2) - 2 * x(n-1) + x(n)
// Filter is (1 - 2z**-1 + z**-2) / (1 - 2z**-1 + 3z**-2)
B = [1 -2 1];
A = [1 -2 3];
x = [1 2 -1 1 -1];
y = [1 2 -3 -7 -9];
res = filter(B, A, x);
assert_checkalmostequal(res, y);
