// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 569 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=569
//
// <-- Short Description -->
//    contour2d does not work for me; however, contour works as
//    expected of contour2d: 2D plot of a set of level lines.


disp("check the result of the  graphic windows");

x  = 0:0.1:1;
f  = sin(2*%pi*x)'*cos(2*%pi*x);
nz = [0,0.1,0.2];
if execstr('contour(x,x,f,nz)','errcatch') <> 0 then pause,end
