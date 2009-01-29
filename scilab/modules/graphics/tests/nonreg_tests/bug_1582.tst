// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1582-->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1582
//
// <-- Short Description -->
// If parametric 3d curve values are out of given ebox of 3d axes, this curve values are netherveless plotted.
// 

xv = 1:10;
yv = rand(1, length(xv))*20-10;
param3d1(xv, yv, ...
  list(zeros(length(yv), 1), 5), flag=[1, 4], theta = 35, ...
  alpha = 45, ebox=[min(xv), max(xv), -1, 1, -5, 5]);   
  
e = gce();
if (e.clip_state <> "clipgrf") then pause; end
