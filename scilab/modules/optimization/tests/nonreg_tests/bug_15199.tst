// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI-Group - Delamarre Cedric
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 15199 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15199
//
// <-- Short Description -->
// Scilab crash when using fsolve for functions containing other functions
//

function y=fy(x)
function z=fz(t)
z=t-x
endfunction
T=fsolve(1,fz)
y=T-x
endfunction

assert_checkequal(fsolve(1,fy), 1);
