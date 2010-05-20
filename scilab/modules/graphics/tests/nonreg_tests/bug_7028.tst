// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre Lando
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 7028 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7028
//
// <-- Short Description -->
// When scilab printed an "axis" graphic handle, string array size for "tics_labels" were erroneous.

// Execute this code :

clf;
plot2d(0:1,0:1,rect=[0 0 1 1],nax=[1 6 1 6]);
a=gca();
p=a.x_ticks.locations;
v0=a.x_ticks.labels;
drawaxis(x=p,y=0.5,dir="u",tics="v",val=v0);
e=gce();


e.xtics_coord=(0:20)/20

// Here you should see someting like that:
//
//Handle of type "Axis" with properties:
//======================================
//[...]
// tics_labels = string array 1x21
//[...]

