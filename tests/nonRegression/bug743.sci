// <-- Non-regression test for bug 743 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=743
//
// <-- Short Description -->
//    Le code suivant tourne en scilab 2.7.2 mais pas en 3.0:
//    x = int32(253);
//    if ((x&int32(1))<>int32(0)) then
//      printf('Test\n');
//    end


// Non-regression test file for bug 743
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2005
// Date : 21 octobre 2005

mode(-1);
clear;

x=int32(253);
if ((x&int32(1))<>int32(0)) then
	affich_result(%T,753);
else
	affich_result(%F,753);
end

clear
