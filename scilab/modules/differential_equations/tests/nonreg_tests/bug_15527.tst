// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15527 -->
//
// <-- Bugzilla URL -->
// https://bugzilla.scilab.org/show_bug.cgi?id=15527
//
// <-- Short Description -->
// Ode outputs incorrect results with %ODEOPTIONS(1)=2

function ydot=f(t, y)
    ydot=y^2-y*sin(t)+cos(t)
endfunction
%ODEOPTIONS=[2,0,0,%inf,0,2,500,12,5,0,-1,-1];
rtol=1e-6;
atol=1e-8;
y=ode(0,0,%pi,rtol,atol,f)
assert_checktrue(size(y,2)>30)
assert_checkalmostequal(y(2,:),sin(y(1,:)),1e-4,[],'element')
