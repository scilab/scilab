// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 10270 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10270
//
// <-- Short Description -->
// execstr(["v = [int16(1)";"]"]) in the console needed entering twice <enter>
// to be actually performed and define v.

// Execute the following in the console:
clear v
execstr(["v=[int16(1)";"]"])
// Then, v should be actually defined
v
