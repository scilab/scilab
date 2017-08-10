// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- LONG TIME EXECUTION -->
//
// <-- Non-regression test for bug 12559 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12559
//
// <-- Short Description -->
// Some variables were not freed and generated important memory leaks.

U = rand(32, 32);
Nit = 10000000;

for it=1:Nit
    U = dct(U);
end
