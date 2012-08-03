// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012-2012 - Scilab Enterprises - bruno.jofret@scilab-enterprises.com
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 11450 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11450
//
// <-- Short Description -->
// When having logarithmic autoticks, ticks are not well sorted


plot2d("ll",1:1000,0.01:0.01:10)
a = gca();
xticks = a.x_ticks.locations;
assert_checkequal(xticks,gsort(xticks, 'g', 'i'));
yticks = a.y_ticks.locations;
assert_checkequal(yticks,gsort(yticks, 'g', 'i'));