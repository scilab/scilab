// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 14249 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14249
//
// <-- Short Description -->
// ctrl-c can be used to stop writing control expression

for i = 1:10
    disp(ii); //oops a typo !
    //press ctrl-c to stop "for" writing
