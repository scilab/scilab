// <-- Non-regression test for bug 2098 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2098
//
// <-- Short Description -->
//    showprofile(test) makes Scilab crash if the function 'test' has not 
//    been prepared for profiling, as demonstrated by the following example:
//
//    -->function test
//    -->disp('test')
//    -->endfunction
//
//    -->test
//
//     test
//
//    -->showprofile(test)  // this line makes Scilab exit silently


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
