// <-- Non-regression test for bug 2407 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2407
//
// <-- Short Description -->
//    stdev and st_deviation are not available for hypermatrix.
//
//    When trying to calculate stdev of an hypermatrix, an error occurs 
//    systematically. The stdev of an hypermatrix should be managed by the %
//    hm_st_deviation function in Scilab.
//    After a quick look at the code of this function, i think i've 
//    understood the origin of the bug. There is a mix between two variables :
//    N and d.
//    For example, at line 8, instruction "p1=prod(dims(1:d-1));" should be 
//    replace by p1=prod(dims(1:N-1));
//    I think that the replacement of d by N in all the function could solve 
//    the problem.
//
//    Regards.


// Non-regression test file for bug 2407
// Author : Scilab Project - Pierre MARECHAL
// Copyright INRIA
// Date : 05 mai 2007

mode(-1);
clear;

a = zeros(3,3,3);
b = stdev(a);

if( b == 0 ) then
	affich_result(%T,2407);
else
	affich_result(%F,2407);
end

clear
