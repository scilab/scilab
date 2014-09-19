// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- unit test for samplef.sci-->
//


//==============================================================================
// Checking error messages
//==============================================================================
fname = "samplef";

err_msg_negative = msprintf(_("%s: Wrong value for input argument #%d: Non-negative values expected.\n"), fname, 3);

err_msg_size_glob = msprintf(_("%s: Wrong size for input argument #%d and #%d: Same size expected.\n"), fname, 2, 3);

err_msg_size_row = msprintf(_("%s: Wrong size for input argument #%d: Same number of rows of the second input argument expected.\n"), fname, 3);

err_msg_size_col = msprintf(_("%s: Wrong size for input argument #%d: Same number of columns of the second input argument expected.\n"), fname, 3);

//========================================
// Error if sum of frequency is negative
n = 100;
X = 1:10;
f = [-10:-1];
func_str = "samplef(n, X, f)";

grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
assert_checkerror(func_str, err_msg_negative);

//========================================
// Error if frequency is not the same size af the
X = 1:10;
f = [1:12];
func_str = "samplef(n, X, f)";

grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
assert_checkerror(func_str, err_msg_size_glob);

//========================================
// Error if row mode on and size of f does not match the number of rows
X = [1 2 3; 4 5 6];
f = [5 10 15];
func_str = "samplef(n, X, f, ""r"")";

grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
assert_checkerror(func_str, err_msg_size_row);

//========================================
// Error if column mode on and size of f does not match the number of columns
X = [1 2 3; 4 5 6];
f = [5 10];
func_str = "samplef(n, X, f, ""c"")";

grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
assert_checkerror(func_str, err_msg_size_col);


//==============================================================================
// Checking Normal behaviours for complete size '*'
//==============================================================================

//========================================
// Tests normalized frequencies
grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
X = [1 2 3];
f = [0.5 0.3 0.2];

samples = samplef(10, X, f);

grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
samples_star = samplef(10, X, f, "*");

//Check both results are identical
assert_checkequal(samples, samples_star);

//========================================
// Tests non normalized frequencies
grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
f = f * 10;

samples = samplef(10, X, f);

grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
samples_star = samplef(10, X, f, "*");

//Check both results are identical
assert_checkequal(samples, samples_star);

//========================================
// Checks 0 frequency value is not taken
grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
f = ones(1, 10);
f(4) = 0;
X = 1:10;

samples = samplef(10000, X, f, "*");
assert_checktrue(isempty(find(samples == 4)));

grand("setsd", 0);
f = zeros(1,10);
f(4) = 1;
samples = samplef(10000, X, f, "*");
assert_checkalmostequal(samples, 4 .* ones(1,10000));

grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
f = ones(1, 10);
f(4) = 0;
X = 1:10;
X = matrix(X, 2, 5);

//No crash for X matrix and f correct size
samples = samplef(100, X, f, "*");
assert_checktrue(isempty(find(samples == 4)));

//========================================
// Checks consistency of size when called without "orient" options
grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
f = [0.1 0.2 0.3; 0.2 0.1 0.1];
X = ["a", "b", "c"; "d", "e", "f"];

samples = samplef(100, X, f);
assert_checkequal(size(samples, 'r'), 1);
assert_checkequal(size(samples, 'c'), 100);

//==============================================================================
// Checking Normal behaviours for row size 'r'
//==============================================================================

//========================================
// Test normalized frequencies
grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
X = [1 2 3; 4 5 6];
f = [0.1 0.9];

samples_r_n = samplef(10, X, f, "r");

grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
samples_one_n = samplef(10, X, f, 1);

//Check both results are identical
assert_checkequal(samples_r_n, samples_one_n);

//========================================
// Test non normalized frequencies
grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
X = [1 2 3; 4 5 6];
f = [1000 9000];

samples_r = samplef(10, X, f, "r");

grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
samples_one = samplef(10, X, f, 1);

//Check both results are identical
assert_checkequal(samples_r, samples_one);

//Normalized and non normalized should have the same behaviour
assert_checkequal(samples_r_n, samples_r);

//========================================
// Checks 0 frequency value is not taken
grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
X = [1 2 3; 4 5 6];
f = [1 0];

samples = samplef(1000, X, f, 1);
assert_checkalmostequal(samples, repmat([1 2 3], 1000, 1));

//==============================================================================
// Checking Normal behaviours for column size 'c'
//==============================================================================

//========================================
// Test normalized frequencies
grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
X = [1 2 3; 4 5 6];
f = [0.1 0.7 0.2];

samples_c_n = samplef(10, X, f, "c");

grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
samples_two_n = samplef(10, X, f, 2);

//Check both results are identical
assert_checkequal(samples_c_n, samples_two_n);

//========================================
// Test non normalized frequencies
grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
X = [1 2 3; 4 5 6];
f = [100 700 200];

samples_c = samplef(10, X, f, "c");

grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
samples_two = samplef(10, X, f, 2);

//Check both results are identical
assert_checkequal(samples_c, samples_two);

//Normalized and non normalized should have the same behaviour
assert_checkequal(samples_c_n, samples_c);

//========================================
// Checks 0 frequency value is not taken
grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
X = [1 2 3; 4 5 6];
f = [1 0 1];

samples = samplef(10000, X, f, "c");
assert_checktrue(isempty(find(samples == 2 | samples == 5)));
