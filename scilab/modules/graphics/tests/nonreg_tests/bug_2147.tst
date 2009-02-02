// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 2147 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2147
//
// <-- Short Description -->
// errbar function do not work in log mode.
// 

t=[0:0.1:2*%pi]';
y=[sin(t) cos(t)]; x=[t t];
plot2d(x,y)
errbar(x,y,0.05*ones(x),0.03*ones(x))
axes = gca();
axes.data_bounds(1,1) = 0.01;
axes.log_flags = "lnn";

// check that errbars are still there
