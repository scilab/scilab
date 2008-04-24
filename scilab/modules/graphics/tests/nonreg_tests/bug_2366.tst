// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2366 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2366
//
// <-- Short Description -->
// If auto_ticks for a certain axis is turned off (e.g. [ "on","on","off" ]) and no
// manual ticks for the axis are set, the label of an arbitrary axis will disappear
// unless the ticks become set.


plot3d( ); //Plotting a surface
axes = gca();
axes.x_label.text = "X";
axes.y_label.text = "Y";
axes.z_label.text = "Z";
axes.auto_ticks = ["on","on","off"]; //Autoticks Z turned off

// check that the 3 labels remains


