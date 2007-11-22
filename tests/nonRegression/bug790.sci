// <-- Non-regression test for bug 790 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=790
//
// <-- Short Description -->
//    Scilab crashes when you enter findobj().
//    I know that it is not usefull but, the bug exists.


// Non-regression test file for bug 790
// Author : Allan CORNET
// Copyright INRIA
 
 // Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 23 Mar 2005

mode (-1);
clear;

ierr1 = execstr('findobj() == []','errcatch');
ierr2 = execstr('findobj(''test'') == []','errcatch');
ierr3 = execstr('findobj(''Param1'',''Param2'') == []','errcatch');

if( (ierr1 == 77) &  (ierr2 == 77) &  (ierr3 == 0) ) then
	affich_result(%T,790);
else
	affich_result(%F,790);
end
