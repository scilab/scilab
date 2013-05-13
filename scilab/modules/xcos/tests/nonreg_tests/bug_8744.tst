// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 8744 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8744
//
// <-- Short Description -->
// Using the modelica initialize gui, produced an OCaML exception while solved
// twice.

xcos(SCI + "/modules/xcos/tests/nonreg_tests/bug_8744.zcos")

// Compilation -> Initialize modelica
// Solve (may produce and error)
// Fix a value
// Solve twice

