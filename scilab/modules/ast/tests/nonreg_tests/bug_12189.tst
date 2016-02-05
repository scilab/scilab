// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 12189 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12189
//
// <-- Short Description -->
// for expression segfault when overwrite increment

for i = 1 : 10
    i = 100;
end
