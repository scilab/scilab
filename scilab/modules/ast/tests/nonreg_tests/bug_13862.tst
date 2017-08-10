// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13862 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13862
//
// <-- Short Description -->
// There is no lazy of 'or' operands in 'if' test

global test;
function r = updateTest()
    global test;
    test = 1;
    r=%f;
endfunction

// short cut
test = 0;
thencase = 0;
elsecase = 0;

if 1 | updateTest() then thencase = 1;else elsecase = 1;end
assert_checkequal(test, 0);
assert_checkequal(thencase, 1);
assert_checkequal(elsecase, 0);

test = 0;
thencase = 0;
elsecase = 0;

if [1 1] | updateTest() then thencase = 1;else elsecase = 1;end

assert_checkequal(test, 0);
assert_checkequal(thencase, 1);
assert_checkequal(elsecase, 0);

// non short cut
test = 0;
thencase = 0;
elsecase = 0;

if 0 | updateTest() then thencase = 1;else elsecase = 1;end

assert_checkequal(test, 1);
assert_checkequal(thencase, 0);
assert_checkequal(elsecase, 1);

test = 0;
thencase = 0;
elsecase = 0;

if [0 1] | updateTest() then thencase = 1;else elsecase = 1;end

assert_checkequal(test, 1);
assert_checkequal(thencase, 0);
assert_checkequal(elsecase, 1);
