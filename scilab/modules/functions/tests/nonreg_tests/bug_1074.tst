// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// <-- Non-regression test for bug 1074 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1074
//
// <-- Short Description -->
//    fun2string()  converts anomalously function calls with more
//    than 1 return argument, and does not retain the function name

mode(-1);
clear;

function foo()
	[a,b,c]=foo1(1)
endfunction

A = fun2string(foo)
B = ["function []=ans()";"  [a,b,c] = foo1(1)";"endfunction"]

if or( A <> B ) then pause,end
