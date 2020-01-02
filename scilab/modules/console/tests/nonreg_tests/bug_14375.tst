// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 14375 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14375
//
// <-- Short Description -->
//    Calling input function with a argument of 64 characters or more crashed Scilab.

a=input("How can this line cause Scilab 6.0.0 Beta to crash when pressing enter?:");
// Please type : 65
// and check that a==65
assert_checkequal(a, 65);
