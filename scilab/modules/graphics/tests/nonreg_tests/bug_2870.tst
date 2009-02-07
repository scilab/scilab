// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2870 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2870
//
// <-- Short Description -->
// plzr demo not correct
// 

x=0:0.1:4*%pi;
subplot(2,1,1);
plot2d(x,[sin(x)' sin(2*x)'],leg="sin x@sin 2x");
axes = gca();
leg = axes.children(2);
subplot(2,1,2);
plot2d(x,cos(x));

// check that legend is within the axes bounds of the upper plot
if (leg.position(1) < 0) then pause; end
if (leg.position(2) < 0) then pause; end
if (leg.position(1) > 1) then pause; end
if (leg.position(2) > 1) then pause; end
