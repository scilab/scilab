// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 12818 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12818
//
// <-- Short Description -->
// Segfault in set function with invalid property values dimension.

f = gcf();
err = execstr("f.closerequestfcn = [""resizeMe()"" ""resizeMe()""]", "errcatch");
assert_checktrue(err <> 0);

err = execstr("set(f, ""closerequestfcn"", [""resizeMe()"" ""resizeMe()""])", "errcatch");
assert_checktrue(err <> 0);
delete(f);