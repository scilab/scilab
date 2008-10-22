// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Jean-Baptiste SILVY <jean-baptiste.silvy@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1448 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1448
//
// <-- Short Description -->
//    Bug detected in 3.1.1 under Windows XP.
//    Bug relative to function rotate
//
//    A positive angle corresponds to a clockwise rotation, then in the 
//    negative mathematical way.
//
//    Jacques-Deric

point1 = [1 ; 0];
point2 = rotate (point1, %pi/2);

if norm( point2 - [0;1] ) >= 10e-10 then pause,end
  
