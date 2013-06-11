// Non-regression test file for bug 1977
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2006
// Date : 23 mai 2006

mode(-1);
clear

execstr("inttype(''hell'')","errcatch");

if lasterror() == "first argument is incorrect" then
    affich_result(%T,1977);
else
    affich_result(%F,1977);
end

clear
