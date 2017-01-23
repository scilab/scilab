// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Siddhartha Gairola
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 14012 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14012
//
// <-- Short Description -->
// Now we can also remove leading and trailing spaces or both.

TAB_CHAR = ascii(9);

// Checking default which removes both leading and trailing spaces.
res1 = stripblanks("    hello    ",%f);
assert_checkequal(res1, "hello");

// Checking that the end is trimmed.
res2 = stripblanks("    hello    ",%t, 1);
assert_checkequal(res2, "    hello");

// Checking that the start is trimmed.
res3 = stripblanks("    hello    ",%t, -1);
assert_checkequal(res3, "hello    ");

// Checking that both start and end are trimmed.
res4 = stripblanks("    hello    ",%t, 0);
assert_checkequal(res4, "hello");

//Checking that only spaces are removed and not tabs.
res5 = stripblanks("    hello    "+TAB_CHAR, %f, 0);
assert_checkequal(res5, "hello    "+TAB_CHAR);

//Checking that only spaces as well as tabs are removed.
res6 = stripblanks("    hello    "+TAB_CHAR, %t, 0);
assert_checkequal(res6, "hello");

//Checking that only leading spaces are removed and no tabs.
res7 = stripblanks("    "+TAB_CHAR+"hello    ", %f, -1);
assert_checkequal(res7, TAB_CHAR+"hello    ");

//Checking that only trailing spaces are removed and no tabs.
res8 = stripblanks("    hello"+TAB_CHAR+"    ", %f, 1);
assert_checkequal(res8, "    hello"+TAB_CHAR);

//Checking that only trailing spaces are removed along with tabs.
res9 = stripblanks("    hello"+TAB_CHAR+"    ", %t, 1);
assert_checkequal(res9, "    hello");

//Checking that only leading spaces are removed along with tabs.
res10 = stripblanks("    "+TAB_CHAR+"hello    ", %t, -1);
assert_checkequal(res10, "hello    ");
