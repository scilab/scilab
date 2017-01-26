// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->

// check gda user_data and resulting gca user_data

d = gda();
d.user_data = "test";
assert_checkequal(d.user_data, "test");

a1=gca();
assert_checkequal(a1.user_data, "test");

xdel(winsid());
assert_checkequal(d.user_data, "test");

a1 = gca();
assert_checkequal(a1.user_data, "test");

d.user_data = "test 2";
assert_checkequal(d.user_data, "test 2");
assert_checkequal(a1.user_data, "test");
scf();
a2 = gca();
assert_checkequal(a2.user_data, "test 2");
xdel(winsid());
