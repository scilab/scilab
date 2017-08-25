// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14544 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14544
//
// <-- Short Description -->
// scatter and scatter3 canceled any upstream drawlater instruction

x = linspace(0,2,200);
y = exp(-x).*cos(10*x) + 0.2*rand(1,length(x));
clf
tic();
// create 2D scatter plot
for i=1:12, subplot(3,4,i), scatter(x,y); end
t1 = toc();
clf
tic();
drawlater
for i=1:12, subplot(3,4,i), scatter(x,y); end
drawnow
t2 = toc();
assert_checktrue(t1>t2);
