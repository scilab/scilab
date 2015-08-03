// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
//
// <-- Non-regression test for bug 10867 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10867
//
// <-- Short Description -->
// Loading a .scg file fails when the legend contains accentuated characters.


// SOD format save/load
plot2d();
legend(["é","è","à"]);
f = gcf();
save("TMPDIR/bug_10867.scg", "f");
delete(gcf());
load("TMPDIR/bug_10867.scg");
// Test the other problems listed in the bug report (for new SOD format)
a = gca();
assert_checkequal(a.children(1).text, ["é";"è";"à"]);
assert_checkequal(a.children(2).children(1).mark_mode, "off");
assert_checkequal(a.children(2).children(2).mark_mode, "on");
assert_checkequal(a.children(2).children(3).mark_mode, "on");
assert_checkequal(a.auto_ticks, ["on" "on" "on"]);
delete(gcf());
