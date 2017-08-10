// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - INRIA - Serge STEER
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//============================================================================================
//=================================dct(A ,isn, dim, incr) ============================================
//==============================================================================================
//*****multiple 1-D transform ******************************************************************
//Real case
A=[0   1   1
   2  -1  -1
   3   4   4
   5   1  -1];
y=matrix(dct(A(:),-1,4,1),size(A));
assert_checkalmostequal(y,dct(A,-1,1),0,100*%eps);
y1=matrix(dct(y(:),1,4,1),size(A));
assert_checkalmostequal(y1,dct(y,1,1),0,100*%eps);

y=matrix(dct(A(:),-1,3,4),size(A));
assert_checkalmostequal(y,dct(A,-1,2),0,100*%eps);
y1=matrix(dct(y(:),1,3,4),size(A));
assert_checkalmostequal(y1,dct(y,1,2),0,100*%eps);

Dims=[5 4 9 5 6];
A=rand(1,prod(Dims));
y=matrix(dct(A,-1,20,1),[5*4 9 5 6]);
assert_checkalmostequal(y,dct(matrix(A,[5*4 9 5 6]),-1,1),0,100*%eps);

y=matrix(dct(A,-1,45,20),[5*4 9*5 6]);
assert_checkalmostequal(y,dct(matrix(A,[5*4 9*5 6]),-1,2),0,100*%eps);
y1=matrix(dct(y(:),1,45,20),size(A));
assert_checktrue(isreal(y1));
assert_checkalmostequal(y1,A,0,100*%eps);

Dims=[5 4 9 5 6];
A=rand(1,prod(Dims));
y=matrix(dct(A(:),-1,[9 5],[20 180]),[5 4 9 5 6]);
y1=dct(matrix(A,[5 4 9 5 6]),-1,[3 4]);
assert_checkalmostequal(y,y1,0,100*%eps);

y1=dct(y(:),1,[9 5],[20 180]);
assert_checkalmostequal(A(:),y1,0,100*%eps);
assert_checktrue(isreal(y1));


Dims=[5 4 9 5 6];
A=rand(1,prod(Dims));
y1=dct(matrix(A,[5 4 9 5 6]),-1,[2 4]);

y=matrix(dct(A(:),-1,[4 5],[5 180]),[5 4 9 5 6]);

assert_checkalmostequal(y,y1,0,100*%eps);

y1=dct(y(:),1,[4 5],[5 180]);
assert_checkalmostequal(A(:),y1,10*%eps,100*%eps);
assert_checktrue(isreal(y1));

//complex case
A=[%i   1   1
   2  -1+3*%i  -1;
   3    4   4;
   5*%i   1  -1];
y=matrix(dct(A(:),-1,4,1),size(A));
assert_checkalmostequal(y,dct(A,-1,1),0,100*%eps);
y1=matrix(dct(y(:),1,4,1),size(A));
assert_checkalmostequal(y1,dct(y,1,1),0,100*%eps);


Dims=[5 4 9 5 6];
A=rand(1,prod(Dims))+%i*rand(1,prod(Dims));
y1=dct(matrix(A,[5 4 9 5 6]),-1,[2 4]);

y=matrix(dct(A(:),-1,[4 5],[5 180]),[5 4 9 5 6]);

assert_checkalmostequal(y,y1,0,100*%eps);

y1=dct(y(:),1,[4 5],[5 180]);
assert_checkalmostequal(A(:),y1,10*%eps,100*%eps);
