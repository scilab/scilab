// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 12769 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12769
//
// <-- Short Description -->
// xset("window", 1) did not set the current axes
//

xset("window", 1);
plot2d("nn", 1);

xset("window", 2);
plot2d("ln", 1);

xset("window", 1);
a=gca();
assert_checkequal(a.log_flags,"nnn");

delete(gcf());
delete(gcf());