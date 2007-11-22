// <-- Non-regression test for bug 1239 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1239
//
// <-- Short Description -->
//    This bug was found by Marek. He says us in the newsgroup:
//
//    It seems that nanmax, nanmin functions are not working well,
//    or at
//    least not according to documentation(they should supposedly
//    ignore %nan
//    values). I get the wollowing results in scilab 3.0:
//
//    -->nanmax([%nan, -1])
//     ans  = Inf
//
//    -->nanmax([%nan, 1])
//     ans  = Inf
//    The same for nanmin. 
//    ========================================================
//
//    Btw, on a scilab cvs I don't see any problem with nanmin.
//
//     Bruno


// Non-regression test file for bug 1239
// Author : Scilab Project - Pierre MARECHAL
// Copyright INRIA
// Date : 21 Mar 2005

mode(-1);
clear;

if( (nanmax([%nan, -1]) <> -1) | (nanmax([%nan, 1]) <> 1) | (nanmin([%nan, -1]) <> -1) | (nanmin([%nan, 1]) <> 1) ) then
	affich_result(%F,1239);
else
	affich_result(%T,1239);
end

clear
