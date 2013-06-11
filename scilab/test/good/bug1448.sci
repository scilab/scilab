// Non-regression test file for bug 1448
// Copyright INRIA
// Scilab Project - Jean-Baptiste Silvy
// Copyright INRIA 2006
// Date : October 20 2006

mode(-1);
clear;

point1 = [1 ; 0];
point2 = rotate (point1, %pi/2),


if ( norm( point2 - [0;1] ) < 10e-10 ) then
    affich_result(%T,1448);
else
    affich_result(%F,1448);
end

clear;

