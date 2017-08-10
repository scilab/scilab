// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Nikhil Goel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14483 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14483
//
// <-- Short Description -->
// figure("Name" ...) should be an alias to figure("Figure_name" ...)
// =============================================================================

// Check if Name is working
f1 = figure("name", "nikhil");

// Check if Figure_name is working
f2 = figure("figure_name", "nikhil");
