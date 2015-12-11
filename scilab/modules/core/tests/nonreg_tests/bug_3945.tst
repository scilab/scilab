//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3945 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3945
//
// <-- Short Description -->
// problem with library extraction
ref=sinm(1);
prot=funcprot();funcprot(0);
//redefine the sinm function
function y=sinm(x),y=x+1,endfunction
assert_checkequal(sinm(1),2);
//restore it
sinm = elementary_functionslib.sinm;
assert_checkequal(sinm(1),ref);
funcprot(prot);
