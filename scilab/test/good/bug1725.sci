// Non-regression test file for bug 1725
// Copyright INRIA
// Scilab Project - Farid Belahcene
// Copyright INRIA 2005
// Date : 09 janvier 2006

mode(-1);
clear

a = mtlb_axis([ 1 500 -22.3 1.4])
test1 =  ( a == [1 500 -22.3 1.4]  )

if ( test1 ) then
    affich_result(%T,1725);
else
    affich_result(%F,1725);
end

clear