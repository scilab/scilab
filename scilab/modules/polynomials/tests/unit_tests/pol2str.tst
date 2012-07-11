// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

assert_checkequal(pol2str(%z), " +z");
assert_checkequal(pol2str([%z %z]), [" +z", " +z"]);

p = poly ([1, 2, 3], "x","coeff");
P = [p, p ,p ; p, p, p];
R = pol2str(P);
REF = [" 1+2*x+3*x^2", " 1+2*x+3*x^2", " 1+2*x+3*x^2";
       " 1+2*x+3*x^2", " 1+2*x+3*x^2", " 1+2*x+3*x^2"];
 
assert_checkequal(R, REF);
 
msgerror = msprintf(gettext("%s: Wrong type for input argument #%d: Polynomial expected.\n"), "pol2str", 1);
assert_checkerror("pol2str(1)", msgerror); 
