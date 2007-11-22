// <-- Non-regression test for bug 2045 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2045
//
// <-- Short Description -->
//    xpolys([],[],-1)
//    kills scilex

// Copyright INRIA
// Scilab Project - Jean-Baptiste Silvy
// Copyright INRIA 2006
// Date : October 27 2006

// should not draw anything
xpolys([],[],-1) ;

e = gce() ;

if ( e.type <> "Axes" ) then
  affich_result(%F,2045) ;
end


xpoly([],[]) ;

// should create an empty polyline
e = gce() ;

if ( e.type <> "Polyline" ) then
  affich_result(%F,2045) ;
end

if ( size(e.data) <> [0,0] ) then
  affich_result(%F,2045) ;
end

// check if scilab don't crash
plot3d ;

e.data = rand(100,2) ;

e.data = [] ;

xdel(winsid()) ;

// check if nothing is broken
xpolys(rand(12,12), rand(12,12) ) ;

e = gce() ;

if ( e.type <> "Compound" ) then
  affich_result(%F,2045) ;
end

if ( size(e.children) <> 12 ) then
  affich_result(%F,2045) ;
end

delete(gca()) ;

xpoly( rand(1,12), rand(1,12) ) ;

e = gce() ;

if ( e.type <> "Polyline" ) then
  affich_result(%F,2045) ;
end

affich_result(%T,2045) ;
