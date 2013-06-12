// Non-regression test file for bug 1859
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2006
// Date : 27 fevrier 2006

mode(-1);
clear

Year  = [2002, 2002];
Month = [   2,    3];
Day   = [  28,    1];

test = datenum(Year, Month, Day);
ref = [731275 ; 731276];

if ~or(test <> ref) then
    affich_result(%T,1859);
else
    affich_result(%F,1859);
end

clear
