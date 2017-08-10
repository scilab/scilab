// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Jean-Baptiste SILVY <jean-baptiste.silvy@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1560 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1560
//
// <-- Short Description -->
//    (Following of bug 1558)
//    In a 3D representation, under the rotation angles [0,270], the third 
//    coordinate z1 of the first point of a segment is ignored, while the 
//    third coordinate z2 of the second point is considered.
//
//    Jacques-Deric

hf = scf (1);
ha = hf.children;
ha.axes_visible = "on";
ha.view = "3d";
ha.box = "on";

xsegs ([0.0,0.0], [1.0,1.0]); 
hs10 = gce ();
hs10.data (1,1) = 1.00;  // x1
hs10.data (1,2) = 0.60;  // y1
hs10.data (2,1) = 0.70;  // x2
hs10.data (2,2) = 0.45;  // y2
hs10.data,

ha.rotation_angles = [0,270];
hs10.data (1,3) =-0.30;  // z1 
hs10.data (2,3) =-0.40;  // z2
hs10.data,

ha.rotation_angles = [0,0];

reference = [ 1, 0.6, -0.3 ; 0.7, 0.45, -0.4 ] ;

// check that the segment z coordinates are modified
if hs10.data <> reference then pause,end
