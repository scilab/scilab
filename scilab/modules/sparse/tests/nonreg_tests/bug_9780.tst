// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 9780 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9780
//
// <-- Short Description -->
// gmres does not run with complex systems

n_eqns=3;
tol=1.e-6;
rstr=n_eqns;
maxi=5000;
M=eye(n_eqns,n_eqns)+%i*eye(n_eqns,n_eqns);
x0=zeros(n_eqns,1);
A=rand(n_eqns,n_eqns)+%i*rand(n_eqns,n_eqns);
b=rand(n_eqns,1);
assert_checktrue(execstr("gmres(A,b,rstr,tol,maxi,M,x0)-[0.4336542-0.8855630*%i;-0.9532019+0.9207990*%i;0.5351043-0.3759549*%i]","errcatch") == 0);
