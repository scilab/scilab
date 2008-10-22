// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3184 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3184
//
// <-- Short Description -->
// legends breaks isoview
 
t=[0:0.1:2*%pi+0.1]';
x=cos(t);y=sin(t);
plot(x,y);plot(2*x,2*y);plot(5*x,5*y)
ha=gca();
ha.isoview = "on";
legends(["Circle with radius 1";..
         "Circle with radius 2";..
         "Circle with radius 5"],..
         [1,5,6],opt="below");

if (ha.isoview <> "on") then pause; end


// check that the axes are still in isoview mode





