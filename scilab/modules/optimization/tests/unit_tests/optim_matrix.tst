// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//
// optim_matrix.tst --
//   Test the optim command when the optimized unknown is a matrix.
//
Leps=8.e-5;
// Test #1
// The expected solution of the problem is A^-1
// Test with norm 2
deff('[f,g,ind]=ndsim1(x,ind)', 'y=A*x-b;f=norm(y,2)^2;g=2*A''*y')
A=rand(2,2);
b=eye(A);
x0=eye(2,2);
[f,X,g]=optim(ndsim1,x0);
if norm(X-inv(A))>Leps then pause,end
// Test #2
// The expected solution of the problem is the solution of Ax=b
deff('[f,g,ind]=ndsim2(x,ind)', 'y=A*x-b;f=sum(abs(y));g=A''*sign(y)')
A=rand(2,2);
b=[1;0];
Ai=inv(A);
[f,x,g]=optim(ndsim2,[1;0],'nd');
if norm(x-Ai(:,1))>Leps then pause,end
// Test #3
deff('[f,g,ind]=ndsim3(x,ind)', [
'y=A*x-b;f=max(abs(y)),sel=abs(y)==f'
'g=A(sel,:)''*sign(y(sel))']);
// TODO : test something ! These are hidden dead bodies...
//A=rand(2,2);
//b=[1;0];
//Ai=inv(A);
//[f,x,g]=optim(ndsim,[1;0],'nd');
//if norm(x-Ai(:,1))>Leps then pause,end

