// <-- Non-regression test for bug 1977 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1977
//
// <-- Short Description -->
//    inttype has stars in error message
//
//
//    -->inttype("hell")
//                    !--error 44
//    ***th argument is incorrect
//
//
//    Francois


// Non-regression test file for bug 1977
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2006
// Date : 23 mai 2006

mode(-1);
clear

execstr('inttype(''hell'')','errcatch');

if lasterror() == 'first argument is incorrect' then
	affich_result(%T,1977);
else
	affich_result(%F,1977);
end

clear
