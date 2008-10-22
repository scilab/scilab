// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Jean-Baptiste SILVY <jean-baptiste.silvy@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2162 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2162
//
// <-- Short Description -->
//    When using plot3d with facets coordinates, it is impossible to draw a single facet.

xf = [0;0;0;0] ;
yf = [0;1;1;0] ;
zf = [0;0;1;1] ; // coordinates of a square

// plot3d routine should be able to draw a single square
plot3d( xf, yf, zf ) ;
e = gce() ;

if e.type <> "Fac3d" then pause,end

