// <-- Non-regression test for bug 1074 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1074
//
// <-- Short Description -->
//    fun2string()  converts anomalously function calls with more
//    than 1 return argument, and does not retain the function name


// Non-regression test file for bug 1057
// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 1er juin 2005

mode(-1);
clear;

function foo()
	[a,b,c]=foo1(1)
endfunction

A = fun2string(foo)
B = ["function []=ans";"  [a,b,c] = foo1(1)";"endfunction"]

if or( A <> B ) then pause,end
