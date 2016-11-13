// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// Run with test_run('randlib', 'grand_hypermat', ['no_check_error_output'])


///////////////////////////////////////////////////////////////////////////////
//
// Dimensions
mat = grand(100, 101, 102, "unf", 0, 1);
assert_checktrue(size(mat) == [100 101 102]);

///////////////////////////////////////////////////////////////////////////////
//
// Generators
// The grand(i, j, ...) should be equal to the first element of grand(i, j, k, ...).

// mt generator
grand("setgen", "mt");
grand("setsd", 0);
expected = grand(4, 6, "def");
grand("setsd", 0); // Resetting the seed to obtain the same results
computed = grand(4, 6, 5, "def");
assert_checkequal(expected, computed(:, :, 1));
grand("setsd", 0);
expected = grand(4, 6, "lgi");
grand("setsd", 0); // Resetting the seed to obtain the same results
computed = grand(4, 6, 5, "lgi");
assert_checkequal(expected, computed(:, :, 1));

// kiss generator
grand("setgen", "kiss");
grand("setsd", 0, 0, 0, 0);
expected = grand(4, 6, "def");
grand("setsd", 0, 0, 0, 0); // Resetting the seed to obtain the same results
computed = grand(4, 6, 5, "def");
assert_checkequal(expected, computed(:, :, 1));
grand("setsd", 0, 0, 0, 0);
expected = grand(4, 6, "lgi");
grand("setsd", 0, 0, 0, 0); // Resetting the seed to obtain the same results
computed = grand(4, 6, 5, "lgi");
assert_checkequal(expected, computed(:, :, 1));

// clcg2 generator
grand("setgen", "clcg2");
grand("setsd", 1, 1);
expected = grand(4, 6, "def");
grand("setsd", 1, 1); // Resetting the seed to obtain the same results
computed = grand(4, 6, 5, "def");
assert_checkequal(expected, computed(:, :, 1));
grand("setsd", 1, 1);
expected = grand(4, 6, "lgi");
grand("setsd", 1, 1); // Resetting the seed to obtain the same results
computed = grand(4, 6, 5, "lgi");
assert_checkequal(expected, computed(:, :, 1));

// clcg4 generator
grand("setgen", "clcg4");
warning("off");
grand("setsd",1,1,1,1);
warning("on");
expected = grand(4, 6, "def");
warning("off");
grand("setsd",1,1,1,1); // Resetting the seed to obtain the same results
warning("on");
computed = grand(4, 6, 5, "def");
assert_checkequal(expected, computed(:, :, 1));
warning("off");
grand("setsd",1,1,1,1); // Resetting the seed to obtain the same results
warning("on");
expected = grand(4, 6, "lgi");
warning("off");
grand("setsd",1,1,1,1); // Resetting the seed to obtain the same results
warning("on");
computed = grand(4, 6, 5, "lgi");
assert_checkequal(expected, computed(:, :, 1));

// urand generator
grand("setgen", "urand");
grand("setsd", 1);
expected = grand(4, 6, "def");
grand("setsd", 1); // Resetting the seed to obtain the same results
computed = grand(4, 6, 5, "def");
assert_checkequal(expected, computed(:, :, 1));
grand("setsd", 1);
expected = grand(4, 6, "lgi");
grand("setsd", 1); // Resetting the seed to obtain the same results
computed = grand(4, 6, 5, "lgi");
assert_checkequal(expected, computed(:, :, 1));
