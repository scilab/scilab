// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3348 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3348
//
// <-- Short Description -->
//    Missing tests in lsqrsolve


a=[1,2,4;
   7,8,3];
b=[10;11];
function y=f1(x,m),y=a*x+b;endfunction
assert_checkfalse(execstr('[xsol,v]=lsqrsolve([100;100;100],f1,2)','errcatch') == 0);

a=[1,7;
   2,8
   4,3];
b=[10;11;-1];
function y=f1(x,m),y=a*x+b;endfunction

assert_checkfalse(execstr('[xsol,v]=lsqrsolve([100;100],f1,3,[-1.d-8,1.d-8,1.d-5,1000,0,100])','errcatch') == 0);
assert_checkfalse(execstr('[xsol,v]=lsqrsolve([100;100],f1,3,[1.d-8,1.d-8,1.d-5,1000,0,100],[0 1])','errcatch') == 0);
