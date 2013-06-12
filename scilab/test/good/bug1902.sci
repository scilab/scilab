// Non-regression test file for bug 1902
// Copyright INRIA
// Scilab Project - Jean-Baptiste Silvy
// Copyright INRIA 2006
// Date : October 25 2006


// draw later part
drawlater() ;

xstring(0,0,["toto","titi";"tatapjp","iiih"]) ;

e = gce() ;

box11 = stringbox(e)

a = gca() ;
a.data_bounds = [-2,-2;7,7] ;

box21 = stringbox(e)

a.view = "3d" ;

box31 = stringbox(e)

a.data_bounds = [0,-1,3;6,7,8] ;

box41 = stringbox(e)

drawnow() ;


clf() ;


// same as first part but wthout drawlater
xstring(0,0,["toto","titi";"tatapjp","iiih"]) ;

e = gce() ;

box12 = stringbox(e)

a = gca() ;
a.data_bounds = [-2,-2;7,7] ;

box22 = stringbox(e)

a.view = "3d" ;

box32 = stringbox(e)

a.data_bounds = [0,-1,3;6,7,8] ;

box42 = stringbox(e)

// bounding box should be the same either we are in drawlater or not.
if (  norm( box11 - box12 ) < 0.02 ..
    & norm( box21 - box22 ) < 0.02 ..
    & norm( box31 - box32 ) < 0.02 ..
    & norm( box41 - box42 ) < 0.02 ) then
    affich_result(%T,1902) ;
else
    affich_result(%F,1902) ;
end

