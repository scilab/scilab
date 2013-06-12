// Non-regression test file for bug 1711
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2005
// Date : 28 décembre 2005

mode(-1);
clear

test1 =  ( datenum(2006,1,4) == 732681 );

if test1 then
    affich_result(%T,1711);
else
    affich_result(%F,1711);
end

clear
