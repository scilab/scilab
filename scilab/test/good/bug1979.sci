// Non-regression test file for bug 1979
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2006
// Date : 23 mai 2006

mode(-1);
clear

a = int32(1);
b = int32(6);
c = a:b;
d = int32([1 2 3 4 5 6]);

if  and( c == d ) then
    affich_result(%T,1979);
else
    affich_result(%F,1979);
end

clear
