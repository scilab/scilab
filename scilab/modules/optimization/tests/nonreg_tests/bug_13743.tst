// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13743 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13743
//
// <-- Short Description -->
// neldermead_configure function failed to set options "-gamma" and "-sigma"

nm = neldermead_new();
ierr = execstr("nm = neldermead_configure(nm, ""-sigma"", 0.25)", "errcatch");
assert_checkequal(ierr, 0);
ierr = execstr("nm = neldermead_configure(nm, ""-gamma"", 0.25)", "errcatch");
assert_checkequal(ierr, 0);
nm = neldermead_destroy(nm);
