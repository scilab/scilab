//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
function r=Err(x),r=norm(x,1),endfunction
rand('normal')

//==========================================================================
//==============================     lsq      ============================== 
//==========================================================================

//Empty matrix
if lsq([],[])<>[] then pause,end
if execstr('lsq([],1)','errcatch')==0 then pause,end
if execstr('lsq(1,[])','errcatch')==0 then pause,end
if execstr('lsq(rand(3,2),rand(2,1))','errcatch')==0 then pause,end

//Small dimensions
//Real full rank fat
A=rand(3,5);b=rand(3,2);
X=lsq(A,b);
if Err(A*X-b)>200*%eps then pause,end
//Real rank deficient fat
A=[1 2 3;1 2 3];b=[4;5];
X=lsq(A,b);
if Err(A'*A*X-A'*b)> 200*%eps then pause,end
//Real  tall
A=[1 2;4 2;0 1];b=[1;1;1];
X=lsq(A,b);
[Q,R]=qr(A);b1=Q'*b;X1=inv(R(1:2,:))*b1(1:2);
if Err(X-X1)>200*%eps then pause,end
//Complex full rank fat
A=rand(3,5)+%i*rand(3,5);b=rand(3,2);
X=lsq(A,b);
if Err(A*X-b)>200*%eps then pause,end

A=rand(3,5);b=rand(3,2)+%i*rand(3,2);
X=lsq(A,b);
if Err(A*X-b)>200*%eps then pause,end

A=rand(3,5)+%i*rand(3,5);b=rand(3,2)+%i*rand(3,2);
X=lsq(A,b);
if Err(A*X-b)>200*%eps then pause,end
//Complex  rank deficient fat
A=[1 2 3;1 2 3]+%i;b=[4;5];
X=lsq(A,b);
A=[1 2 3;1 2 3];b=[4;5]+%i;
X=lsq(A,b);
if Err(A'*A*X-A'*b)>200*%eps then pause,end

if Err(A'*A*X-A'*b)>200*%eps then pause,end
A=[1 2 3;1 2 3]+%i;b=[4;5]+%i;
X=lsq(A,b);
if Err(A'*A*X-A'*b)>1000*%eps then pause,end

//Complex  full rank tall
A=[1 2;4 2;0 1]+%i;b=[1;1;1];
X=lsq(A,b);
[Q,R]=qr(A);b1=Q'*b;X1=inv(R(1:2,:))*b1(1:2);
if Err(X-X1)>200*%eps then pause,end

A=[1 2;4 2;0 1];b=[1;1;1]+%i;
X=lsq(A,b);
[Q,R]=qr(A);b1=Q'*b;X1=inv(R(1:2,:))*b1(1:2);
if Err(X-X1)>200*%eps then pause,end

A=[1 2;4 2;0 1]+%i;b=[1;1;1]+%i;
X=lsq(A,b);
[Q,R]=qr(A);b1=Q'*b;X1=inv(R(1:2,:))*b1(1:2);
if Err(X-X1)>200*%eps then pause,end



//LArge dimension
//Real full rank fat
A=rand(3,50);b=rand(3,2);
X=lsq(A,b);
if Err(A*X-b)>200*%eps then pause,end
//Real full rank tall
A=rand(50,3);b=rand(50,2);
X=lsq(A,b);
[Q,R]=qr(A);b1=Q'*b;X1=inv(R(1:3,:))*b1(1:3,:);
if Err(X-X1)>200*%eps then pause,end

//Complex full rank fat
A=rand(3,50)+%i;b=rand(3,2);
X=lsq(A,b);
if Err(A*X-b)>200*%eps then pause,end
A=rand(3,50);b=rand(3,2)+%i;
X=lsq(A,b);
if Err(A*X-b)>200*%eps then pause,end

A=rand(3,50);b=rand(3,2)+%i;
X=lsq(A,b);
if Err(A*X-b)>200*%eps then pause,end
//Complex full rank tall
A=rand(50,3)+%i;b=rand(50,2);
X=lsq(A,b);
[Q,R]=qr(A);b1=Q'*b;X1=inv(R(1:3,:))*b1(1:3,:);
if Err(X-X1)>200*%eps then pause,end

A=rand(50,3);b=rand(50,2)+%i;
X=lsq(A,b);
[Q,R]=qr(A);b1=Q'*b;X1=inv(R(1:3,:))*b1(1:3,:);
if Err(X-X1)>200*%eps then pause,end
A=rand(50,3)+%i;b=rand(50,2)+%i;
X=lsq(A,b);
[Q,R]=qr(A);b1=Q'*b;X1=inv(R(1:3,:))*b1(1:3,:);
if Err(X-X1)>200*%eps then pause,end


