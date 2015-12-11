//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
function y=f(t)
    y=exp(-t^2);
endfunction

r = calerf(1,0);
ref = 2/sqrt(%pi)*intg(0,1,f);
assert_checkalmostequal(r, ref);

