// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 4925 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4925
//
// <-- Short Description -->
// this example with optim crashs 'optim' on windows (release mode)
// 
// =============================================================================
xref=[1;2;3];
x0=[1;-1;1];
function [f,g,ind] = cost(x,ind)
  f=0.5*norm(x-xref)^2;
  g=x-xref;
endfunction
[f,xopt]=optim(cost,x0,imp=3)
// =============================================================================
