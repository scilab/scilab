// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 11136 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11136
//
// <-- Short Description -->
// A graphic export was crashing when using legends


driver("GIF")
xinit(TMPDIR+"/plop.gif")
t=0:0.1:2*%pi;
plot2d(t,[cos(t'),cos(2*t'),cos(3*t')],[-1,2 3]);
legends(['cos(t)';'cos(2*t)';'cos(3*t)'],[-1,2 3],opt="lr")
xend();

assert_checktrue(isfile(TMPDIR+"/plop.gif"));
