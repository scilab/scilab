// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 632 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=632
//
// <-- Short Description -->
//    The function "lsqrsolve" makes the whole Scilab crash.
//    It even does it with the example cited in the Scilab 
//    documentation:
//
//    // Data fitting problem
//    // 1 build the data
//    a=34;b=12;c=14;
//    deff('y=FF(x)','y=a*(x-b)+c*x.*x');
//    X=(0:.1:3)';Y=FF(X)+100*(rand()-.5);
//
//    //solve
//    function e=f1(abc,m)
//      a=abc(1);b=abc(2),c=abc(3),
//      e=Y-(a*(X-b)+c*X.*X);
//    endfunction
//    [abc,v]=lsqrsolve([10;10;10],f1,size(X,1));
//    abc
//    norm(v)


// Data fitting problem
// 1 build the data
//exec("632.sce");

// this test will fail if scilab crashes

a = 34;
b = 12;
c = 14;

deff('y=FF(x)','y=a*(x-b)+c*x.*x');
X=(0:.1:3)';
Y=FF(X)+100*(rand()-.5);

//solve

function e=f1(abc,m)
	a = abc(1);
	b = abc(2);
	c = abc(3);
	e = Y-(a*(X-b)+c*X.*X);
endfunction

[abc,v] = lsqrsolve([10;10;10],f1,size(X,1));
if norm(abc - [ 34.  ; 12.849045 ; 14.   ] ) > 0.1 then pause,end
if norm(v) - 2.814D-13 > 0.1 then pause,end
