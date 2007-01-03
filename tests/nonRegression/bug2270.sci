// Non-regression test file for bug 2270
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2007
// Date : 3 janvier 2007

mode(-1);
clear;

execstr('datenum(2006,12,32)','errcatch');
error_str = lasterror();

if stripblanks(error_str) <> "The third argument must be between 1 and 31" then
	affich_result(%F,2270);
else
	affich_result(%T,2270);
end

clear
