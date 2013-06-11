// Non-regression test file for bug 2039
// Author : Scilab Project - Pierre MARECHAL
// Copyright INRIA
// Date : 28 june 2006

mode(-1);
clear;

if lasterror(%F) == [] then
    affich_result(%T,2039);
else
    affich_result(%F,2039);
end

clear
