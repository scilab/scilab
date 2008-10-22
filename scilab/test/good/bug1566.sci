// Non-regression test file for bug 1566
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2005
// Date : 28 octobre 2005

mode(-1);
clear;

err = execstr('TCL_SetVar(''A'',1+%i)','errcatch');

if (err== 999) then
	affich_result(%T,1566);
else
	affich_result(%F,1566);
end

lasterror();

clear
