//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 9788 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9788
//
// <-- Short Description -->
// neldermead did not print warning messages when maxfuneval or maxiter was attained.
// The status could only be retrieved in the -status field.
//

function [f, index] = quadratic(x, index)
    f = x(1)^2 + x(2)^2;
endfunction

x0 = [1; 1];

nm = neldermead_new ();
nm = neldermead_configure(nm, "-numberofvariables", 2);
nm = neldermead_configure(nm, "-function", quadratic);
nm = neldermead_configure(nm, "-x0", x0);
nm = neldermead_search(nm);
nm = neldermead_configure(nm, "-maxfunevals", 10);
nm = neldermead_search(nm, 0);
nm = neldermead_search(nm, "off");
nm = neldermead_search(nm, 1);
nm = neldermead_search(nm, "on");
nm = neldermead_configure(nm, "-maxiter", 2);
nm = neldermead_search(nm, 0);
nm = neldermead_search(nm, "off");
nm = neldermead_search(nm, 1);
nm = neldermead_search(nm, "on");

refMsg = msprintf(_("%s: Wrong value for input argument #%d: ""%s"", ""%s"", %d or %d expected.\n"),..
"neldermead_search", 2, "off", "on", 0, 1);
assert_checkerror("neldermead_search(nm, ""k"");", refMsg);
assert_checkerror("neldermead_search(nm, 2);", refMsg);
