// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- NOT FIXED --> 6.0.1 -> 6.0.2
//
// <-- Non-regression test for bug 7919 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/7919
//
// <-- Short Description -->
// %inf*0 returned 0 instead of %nan

assert_checkequal([%inf*0 0*%inf], [%nan %nan]);
