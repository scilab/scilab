// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Jean-Baptiste SILVY <jean-baptiste.silvy@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2045 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2045
//
// <-- Short Description -->
//    xpolys([],[],-1)
//    kills scilex

// should not draw anything
xpolys([],[],-1) ;
entity = gce() ;
if ( entity.type <> "Axes" ) then pause,end
xpoly([],[]) ;

// should create an empty polyline
entity = gce() ;
if ( entity.type <> "Polyline" )  then pause,end
if ( size(entity.data) <> [0,0] ) then pause,end

// check if scilab don't crash
plot3d ;
entity.data = rand(100,2) ;
entity.data = [] ;
xdel(winsid()) ;

// check if nothing is broken
xpolys(rand(12,12), rand(12,12) ) ;
entity = gce();

if ( entity.type <> "Compound" )   then pause,end
if ( size(entity.children) <> 12 ) then pause,end

delete(gca());
xpoly( rand(1,12), rand(1,12) ) ;
entity = gce();
if ( entity.type <> "Polyline" )   then pause,end
