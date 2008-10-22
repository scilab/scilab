// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2210 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2210
//
// <-- Short Description -->
// Display error when merging several 3d polylines.
// In 3d mode, some unwanted segment are drawn from the polylines.
// In 2d mode, some unwanted curves somtimes appears.
// 

x = 1:3 ;
param3d(x,x,x) ;

y = 2:5 ;
param3d(y,y,y+1); 

a =gca() ;
a.rotation_angles = [60,10] ; // bug in 3D
a.view = "2d" ; // bug in 2D   

// Check that there is only two displayed lines.


