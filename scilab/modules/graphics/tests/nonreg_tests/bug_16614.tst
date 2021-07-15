// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 16614 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16614
//
// <-- Short Description -->
// replot() failed to reframe a figure with xstring contents when it is not the
// current figure

f1 = scf(1);
clf
x1 = 0:0.01:10;
plot(x1, sin(x1));
xstring(16,5,"text more stuff");
gce().text_box_mode = "centered";
a1 = gca();
replot();

scf(2); clf
plot(1:10:1000)

replot(f1)
assert_checkalmostequal(f1.children.data_bounds, [0, -0.9999971; 18.832787,5.3652174],1e-7);
