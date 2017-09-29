// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI Group - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK REF -->
// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 15228 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15228
//
// <-- Short Description -->
// get() returned a wrong error message

plot2d()
e = gce();
e = e.children(1);
refMsg = msprintf(_("%s: Wrong size for argument #%d: scalar string expected.\n"), "get", 2);
assert_checkerror("get(e, [""background"" ""user_data""])", refMsg);
