// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2378 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2378
//
// <-- Short Description -->
//     fun2string produces erroneous output (an extra 2 character appears) when there is a comment after an assignment.

function []=f()                                          
a=b//                                   
endfunction   

t=fun2string(f);
if or(stripblanks(t)<>["function []=ans()";"a = b//";"endfunction"]) then pause,end
