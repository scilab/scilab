// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI - Delamarre Cedric
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15300 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15300
//
// <-- Short Description -->
// Crash when the function name is forgotten!

errmsg = gettext(_("Can not assign multiple value in a single variable"));
assert_checkerror("A=(1,1)", errmsg);
