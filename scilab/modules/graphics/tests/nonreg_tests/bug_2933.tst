// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2933 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2933
//
// <-- Short Description -->
// The black rectangle can disappear when you resize graphics window


subplot(211);
t=1:10;plot2d(t,t.^2);
subplot(223);
plot3d();
subplot(224);
plot2d();
xfrect(1,0,3,1);
a=get("current_axes");
 

