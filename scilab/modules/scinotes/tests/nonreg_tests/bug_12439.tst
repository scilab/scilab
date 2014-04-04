// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 12439 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12439
//
// <-- Short Description -->
// edit_error returned a wrong message when there was no last recorded error.

refMsg = _("There is no recorded error");

assert_checkequal(edit_error(), refMsg);
