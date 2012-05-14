// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Jean-Baptiste SILVY <jean-baptiste.silvy@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1682 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1682
//
// <-- Short Description -->
//    Scilab 4.0 RC1 under Windows XP
//    The field Figure.rotation_style="multiple" does not work correctly.
//
//    The rotation of the axes of the first system by the change of the field
//    Axes.rotation_angles does not force the rotation of the second one.
//
//    When making a manual rotation, it works.
//
//    Jacques-Deric

disp ("4.21.51A");
hf                = scf (1);
hf.figure_name    = "4.21.51A";
hf.background     = 32;
hf.rotation_style = "multiple";

subplot (1, 2, 1);
xfrect (0, 1, 1, 1);
hr1               = gce ();
hr1.background    = 2;
hr1.fill_mode     = "on";
ha1               = hr1.parent; // get the axes

subplot (1, 2, 2);
xfrect (0, 1, 1, 1);
hr2               = gce ();
hr2.background    = 3;
hr2.fill_mode     = "on";
ha2               = hr2.parent; // get the axes

newAngles           = [60,200] ;
ha1.rotation_angles = newAngles ;

// check if both axes are turned
if ( (ha1.rotation_angles <> newAngles) | (ha2.rotation_angles <> newAngles) ) then pause,end
xdel(winsid());