// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge STEER
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 8050 -->
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
// <-- NO TRY CATCH --> 
// 
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8050
//
// <-- Short Description -->
// weird error message
setlanguage('en');

errcatch(-1,"continue")
function foo,[,endfunction

errclear(-1);errcatch(-1,"continue")
function foo
[
endfunction






