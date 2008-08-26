// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 3402 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3402
//
// <-- Short Description -->
// Error messages when issuing a "plot3d()" after a "plot()".
// 

// plot somthing which use the whole colormap
t=0:%pi/20:2*%pi;
plot([t ;t],[sin(t) ;cos(t)],'xdat',[1:2])

// set a smaller color map
// should not generate errors
f = gcf();
status = execstr("f.color_map = jetcolormap(12);",'errcatch');
if (status <> 0) then pause; end


