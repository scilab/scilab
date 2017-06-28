// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
// <-- INTERACTIVE TEST -->
// <-- CLI SHELL MODE -->
// <-- Non-regression test for bug 14399 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14399
//
// <-- Short Description -->
// Whereami : wrong information (line numbers)
function F1(a, b, c)
  x1 = 1;
  x2 = 1;
  x3 = 1;
  pause;
  y1 = 3;
endfunction

function F()
  z = 1;
  F1(1,2,3);
endfunction

F();

// call where in pause
[l, f]=where()
// lines number must be 0 5 3

// call F() then where
F()
[l, f]=where()
// lines number must be 0 5 3 0 5 3

// call resume then where
resume
[l, f]=where()
// lines number must be 0 5 3
