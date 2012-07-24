// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 11396 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11396
//
// <-- Short Description -->
// Operation toto([])=[] with toto being an mlist fails.


function a=%s_i_toto(i,b,a),
  if i<>[]&b<>[] then error("Invalid index");end;
endfunction



A=mlist(["toto","a"],1:4);
B=A;
A([])=[];
assert_checkequal(A,B);

