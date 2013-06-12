// Non-regression test file for bug 1732
// Copyright INRIA
// Scilab Project - Jean-Baptiste Silvy
// Copyright INRIA 2006
// Date : 18 janvier 2006

mode(-1);
clear;

ref = [1,0,5] ;

ha = gca () ;
ha.axes_visible="on" ;
ha.view = "3d" ;
ha.sub_ticks = ref ;
res1 = ha.sub_ticks ;

// mask Z axis
ha.rotation_angles = [1,30];
res2 = ha.sub_ticks ;


// check the sub_ticks were not modified
if ( res1 == ref & res2 == ref  ) then
    affich_result(%T,1732);
else
    affich_result(%F,1732);
end

clear;

