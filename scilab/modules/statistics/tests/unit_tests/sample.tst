// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- unit test for sample.sci-->
//

//==============================================================================
// Checking error messages
//==============================================================================
fname = "sample";
err_msg_glob = msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"), fname, 2, 3);

err_msg_orient = msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', %d or %d expected.\n"), fname, 3, "*", "r", "c", 1, 2);

//========================================
// Error for the number of input arguments
n = 100;
X = 1:10;
orient = "*";

func_str = "sample(n)";

grand("setsd",0); //Sets the randum number to seed 0 for reproductibility
assert_checkerror(func_str, err_msg_glob);

//========================================
// Error for the value of orient
n = 100;
X = 1:10;
orient = "5";

func_str = "sample(n, X, orient)";

grand("setsd",0); //Sets the randum number to seed 0 for reproductibility
assert_checkerror(func_str, err_msg_orient);

//==============================================================================
// Checking normal behaviour for complete size '*'
//==============================================================================

//========================================
// Check default call
n = 100;
X = 1:10;

grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
samples = sample(n, X);

grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
samples_star = sample(n, X, "*");

assert_checkequal(samples, samples_star);

//========================================
// Check size consistency
X = X';

grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
samples_transposed = sample(n, X);

assert_checkequal(samples, samples_transposed);

//==============================================================================
// Checking normal behaviour for complete size 'r'
//==============================================================================

//========================================
// Check default call
n = 100;
X = [1 2 3; 4 5 6];

grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
samples_r_n = sample(n, X, "r");

grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
samples_one_n = sample(n, X, 1);

assert_checkequal(samples_r_n, samples_one_n);

//==============================================================================
// Checking normal behaviour for complete size 'c'
//==============================================================================

//========================================
// Check default call
n = 100;
X = [1 2 3; 4 5 6];

grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
samples_c_n = sample(n, X, "c");

grand("setsd", 0); //Sets the randum number to seed 0 for reproductibility
samples_two_n = sample(n, X, 2);

assert_checkequal(samples_c_n, samples_two_n);
