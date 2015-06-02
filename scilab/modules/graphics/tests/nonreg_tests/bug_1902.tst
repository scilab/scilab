// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Jean-Baptiste SILVY <jean-baptiste.silvy@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1902 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1902
//
// <-- Short Description -->
//    in drawlater mode the bounding box of strings may be wrong

// draw later part
drawlater() ;

xstring(0,0,["toto","titi";"tatapjp","iiih"]) ;

e             = gce();
box11         = stringbox(e);
a             = gca() ;
a.data_bounds = [-2,-2;7,7] ;
box21         = stringbox(e);
a.view        = "3d";
box31         = stringbox(e);
a.data_bounds = [0,-1,3;6,7,8];
box41         = stringbox(e);
box51         = stringbox("toto", 1, 1, 90);
drawnow();
clf();

// same as first part but wthout drawlater
xstring(0,0,["toto","titi";"tatapjp","iiih"]) ;
sleep(200);//Wait for MVC update

e             = gce();
box12         = stringbox(e);
a             = gca();
a.data_bounds = [-2,-2;7,7] ;
box22         = stringbox(e);
a.view        = "3d";
box32         = stringbox(e);
a.data_bounds = [0,-1,3;6,7,8] ;
box42         = stringbox(e);
box52         = stringbox("toto", 1, 1, 90);

// bounding box should be the same either we are in drawlater or not.
if (  norm( box11 - box12 ) >= 0.1 ..
    | norm( box21 - box22 ) >= 0.1 ..
    | norm( box31 - box32 ) >= 0.1 ..
    | norm( box41 - box42 ) >= 0.1 ..
| norm( box51 - box52 ) >= 0.1 )  then pause,end
xdel(winsid());
