// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 15523 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15523
//
// <-- Short Description -->
//    %ODEOPTIONS(1)=2 didn't work with solvers 'rk' and 'rkf'
// =============================================================================

function ydot=f(t, y)
    ydot=y^2-y*sin(t)+cos(t)
endfunction
%ODEOPTIONS=[2,0,0,%inf,0,2,500,12,5,0,-1,-1];
rtol=1e-5;
atol=1e-7;
yrkf=ode("rkf",0,0,%pi,rtol,atol,f);
yrk=ode("rk",0,0,%pi,rtol,atol,f);
assert_checktrue(size(yrkf,2)>1);
assert_checktrue(size(yrk,2)>1);
assert_checkalmostequal(yrk(2,:),sin(yrk(1,:)),1e-4,1e-4,"element")
assert_checkalmostequal(yrkf(2,:),sin(yrkf(1,:)),1e-4,1e-4,"element")
