// Non-regression test file for bug 2098
// Author : Scilab Project - Pierre MARECHAL
// Copyright INRIA
// Date : 05 mai 2007

mode(-1);
clear;

function test
	disp('test');
endfunction

ierr = execstr('showprofile(test)','errcatch');

if( ierr == 1000 ) then
	affich_result(%T,2098);
else
	affich_result(%F,2098);
end

clear
