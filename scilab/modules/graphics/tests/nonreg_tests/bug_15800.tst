// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 15800 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15800
//
// <-- Short Description -->
// elementwise assignment in a vector of handles failed

clf
plot([[1 2]; [1 3]], [[3 4]; [4 5]], "o");
e = gce();
h = e.children;
h.mark_background = h.mark_foreground ;
assert_checkequal(h.mark_background, h.mark_foreground);
set(h, "mark_background", 2*h.mark_foreground);
assert_checkequal(h.mark_background, 2*h.mark_foreground);
