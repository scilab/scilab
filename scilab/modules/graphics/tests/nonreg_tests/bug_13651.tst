// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- Non-regression test for bug 13651 -->
//
// <-- TEST WITH GRAPHIC -->
//
// No relevant console output to be tested:
// <-- NO CHECK REF -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13651
//
// <-- Short Description -->
//    copying an axes in a uicontrol frame was not possible

driver null; // We don't need to render anything
plot2d()
ax1 = gca();
ax1.tag = "plot2d example";
f2 = scf();
fr = uicontrol(f2, "style", "frame");
copy(ax1, fr);

assert_checkequal(fr.children(1).type, "Axes");
assert_checkequal(fr.children(1).tag, "plot2d example");
