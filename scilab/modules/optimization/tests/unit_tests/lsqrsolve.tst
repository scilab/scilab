// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

a=[1,7;
   2,8
   4 3];
b=[10;11;-1];

function y=f1(x,m),y=a*x+b;endfunction

[xsol,v]=lsqrsolve([100;100],f1,3);
if size(xsol) <> [2 1] then pause,end
if size(v) <> [3 1] then pause,end

function y=fj1(x,m),y=a;endfunction
[xsol,v]=lsqrsolve([100;100],f1,3,fj1);

// Data fitting problem
// 1 build the data
a=34;b=12;c=14;
deff('y=FF(x)','y=a*(x-b)+c*x.*x');
X=(0:.1:3)';Y=FF(X)+100*(rand()-.5);

//solve
function e=f2(abc,m)
  a=abc(1);b=abc(2),c=abc(3),
  e=Y-(a*(X-b)+c*X.*X);
endfunction

[abc,v]=lsqrsolve([10;10;10],f2,size(X,1));
if size(abc,'*') <> 3 then pause,end
if norm(v) < %eps then pause,end

