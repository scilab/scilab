// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15420 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15420
//
// <-- Short Description -->
// Insertion at negative index does not trigger an error

//scalar
A = 1;
err = execstr("A(-10) = 1;", "errcatch");
assert_checktrue(err <> 0);

//matrix
A = rand(10,10);
err = execstr("A(-10) = 1;", "errcatch");
assert_checktrue(err <> 0);
err = execstr("A(1, -10) = 1;", "errcatch");
assert_checktrue(err <> 0);

//array of non native types
A = {1 2 3 4};
err = execstr("A(-10) = {1};", "errcatch");
assert_checktrue(err <> 0);

