// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13491 -->
//
// <-- Bugzilla URL -->
//http://bugzilla.scilab.org/show_bug.cgi?id=13491
//
// <-- Short Description -->
// intg now properly returns when the user function produces an error.
ieee(0);
deff("y = f(x)", "y = sin(x) ./ x");
// This function is going to be evaluated by intg at the midpoint of the interval [-1; 1], so it will produce a "Division by zero" error.
refMsg = [msprintf(_("Division by zero...\n"));
msprintf(_("%s: An error occured in ''%s'' subroutine."), "intg", "dqags")];
assert_checkerror("intg(-1, 1, f)", refMsg);

deff("y = g(x)", "y = b");
// In any evaluation point, 'b' is unknown so the function will yield an error.
refMsg = msprintf(_("Undefined variable: %s\n"), "b");
assert_checkerror("intg(-1, 1, g)", refMsg);
