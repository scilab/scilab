// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// This test is to show priority between == and <> operator
// the disp are done on purpose to track the calls.

function b=a(x)
  disp("Compute "+x)
  b=tlist(["tut", "val"], %t)
endfunction


function c=%tut_o_tut(a,b)
  disp("Calling %tut_o_tut")
  c = b
endfunction

function c=%tut_n_tut(a,b)
 disp("Calling %tut_n_tut")
 c = b
endfunction

// Compute 1
// Compute 2
// Calling %tut_o_tut
// Compute 3
// Compute 4
// Calling %tut_o_tut
// Calling %tut_n_tut
(a("1") == a("2")) <> (a("3")==a("4"));



// Compute 1
// Compute 2
// Calling %tut_o_tut
// Compute 3
// Calling %tut_n_tut
// Compute 4
// Calling %tut_o_tut
a("1") == a("2") <> a("3")==a("4");