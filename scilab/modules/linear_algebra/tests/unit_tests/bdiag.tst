//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
function r=Err(x),r=norm(x,1),endfunction
rand('normal')


//==========================================================================
//==============================   bdiag      ============================== 
//==========================================================================
assert_checkequal(bdiag([]),[]);
[ab,x]=bdiag([]);
assert_checkequal(ab,[]);
assert_checkequal(x,[]);


[ab,x,bs]=bdiag([]);
assert_checkequal(ab,[]);
assert_checkequal(x,[]);
assert_checkequal(bs,[]);

assert_checktrue(execstr('bdiag([1 2;3 4;5 6])','errcatch')<>0);

//Small dimension
//---------------
//Real case
e=1.d-1;
A=[1 1  2 3 4 5
   0 1  6 7 8 9
   0 0  1 e 3 1
   0 0 -e 1 5 9
   0 0  0 0 2 e
   0 0  0 0 0 3];
X1=[0.5,0.3,0,0.3,0.3,0.2;
   1,0.6,0.5,0.1,0.7,0.4;
   0.7,0.1,0.4,0.6,0.1,1;
   0,0.6,0.2,0.3,0.4,0.5;
   0.6,0.7,0.5,0.7,0.7,0.5;
   0.3,0.3,0.4,0.5,0.9,0.6];
A=inv(X1)*A*X1;

Ab1=bdiag(A);
assert_checkequal(triu(Ab1,-1),Ab1);

[Ab2,X]=bdiag(A);
assert_checkequal(Ab2,Ab1);
assert_checkalmostequal(inv(X)*A*X,Ab2,sqrt(%eps),0,"matrix");

[Ab2,X,bs]=bdiag(A);
assert_checkequal(Ab2,Ab1);
assert_checkalmostequal(Ab2,inv(X)*A*X,sqrt(%eps),0,"matrix");
assert_checkequal(sum(bs),size(A,1));
assert_checktrue(and(bs>0));

[Ab2,X,bs]=bdiag(A,1);
assert_checkalmostequal(Ab2,inv(X)*A*X,sqrt(%eps),0,"matrix");
assert_checkequal(bs,size(A,1));


//Complex case
e=1.d-1;
A=[1 1  2 3 4 5
   0 1  6 7 8 9
   0 0  1 e 3 1
   0 0 -e 1 5 9
   0 0  0 0 2 e
   0 0  0 0 0 3];
X1=[0.5,0.3,0,0.3,0.3,0.2;
   1,0.6,0.5,0.1,0.7,0.4;
   0.7,0.1,0.4,0.6,0.1,1;
   0,0.6,0.2,0.3,0.4,0.5;
   0.6,0.7,0.5,0.7,0.7,0.5;
   0.3,0.3,0.4,0.5,0.9,0.6]+%i*eye(A);
A=inv(X1)*A*X1;

Ab1=bdiag(A);
assert_checkequal(triu(Ab1,-1),Ab1);
[Ab2,X]=bdiag(A);
assert_checkequal(Ab2,Ab1);
assert_checkalmostequal(Ab2,inv(X)*A*X,sqrt(%eps),0,"matrix");

[Ab2,X,bs]=bdiag(A);
assert_checkequal(Ab2,Ab1);
assert_checkalmostequal(Ab2,inv(X)*A*X,sqrt(%eps),0,"matrix");
assert_checkequal(sum(bs),size(A,1));
assert_checktrue(and(bs>0));
//Large dimension
//---------------
//Real case
A=rand(25,25);
Ab1=bdiag(A);
assert_checkequal(triu(Ab1,-1),Ab1);
[Ab2,X]=bdiag(A);
assert_checkequal(Ab2,Ab1);
assert_checkalmostequal(Ab2,inv(X)*A*X,sqrt(%eps),0,"matrix");

[Ab2,X,bs]=bdiag(A);
assert_checkequal(Ab2,Ab1);
assert_checkalmostequal(Ab2,inv(X)*A*X,sqrt(%eps),0,"matrix");
assert_checkequal(size(bs,2),1);
assert_checkequal(sum(bs),size(A,1));
assert_checktrue(and(bs>0));

//Complex case
A=rand(25,25)+%i*rand(25,25);
Ab1=bdiag(A);
assert_checkequal(triu(Ab1,-1),Ab1);
[Ab2,X]=bdiag(A);
assert_checkalmostequal(Ab2,Ab1);
assert_checkalmostequal(Ab2,inv(X)*A*X,sqrt(%eps),0,"matrix");

[Ab2,X,bs]=bdiag(A);
assert_checkequal(Ab2,Ab1);
assert_checkalmostequal(Ab2,inv(X)*A*X,sqrt(%eps),0,"matrix");
assert_checkequal(size(bs,2),1);
assert_checkequal(sum(bs),size(A,1));
assert_checktrue(and(bs>0));

