// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 4309 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4309
//
// <-- Short Description -->
// Under Scilab5 the dashed lines are not well displayed. They are reinitialized for each segment.
// 

t=linspace(0,2*%pi,200);//a polyline with many points
plot(t,sin(t));
e=gce();e=e.children;
e.line_style=2;// dash selected but not displayed 
//In fact the dash seems to be reinitialized for each segment
clf();t=linspace(0,2*%pi,80);plot(t,sin(t));e=gce();e=e.children;e.line_style=2;
//one can remark that the dashes are irregular

