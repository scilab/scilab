// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//Find x in R^6 such that:
//C1*x = b1 (3 equality constraints i.e me=3)
C1= [1,-1,1,0,3,1;
    -1,0,-3,-4,5,6;
     2,5,3,0,1,0];
b1=[1;2;3];
//C2*x <= b2 (2 inequality constraints)
C2=[0,1,0,1,2,-1;
    -1,0,2,1,1,0];
b2=[-1;2.5];

//and minimize 0.5*x'*Q*x + p'*x with
p=[1;2;3;4;5;6]; Q=eye(6,6);
//No initial point is given;
C=[C1;C2] ; //
b=[b1;b2] ;  //
me=3;
[x1, iact, iter, f]=qpsolve(Q,p,C,b,[],[],me);
if or(iact<>[1;3;2;4;0]) then pause,end
if x1'*p+x1'*Q*x1/2>- 14.843 then pause,end
if or(abs(C(1:4,:)*x1-b(1:4))>100*%eps)  then pause,end
if C(5,:)*x1>b(5) then pause,end

//sparse
[x1, iact, iter, f]=qpsolve(Q,p,sparse(C),b,[],[],me);
if or(iact<>[1;3;2;4;0]) then pause,end
if x1'*p+x1'*Q*x1/2>- 14.843 then pause,end
if or(abs(C(1:4,:)*x1-b(1:4))>100*%eps)  then pause,end
if C(5,:)*x1>b(5) then pause,end

//second test
Q = [0.5 -1; -1 3]; //positive-defined
p = [0; 0];
C = [1 1];
b = 1;
me = 1;

x1=qpsolve(Q,p,C,b,[],[],me);
if or(x1-[8;3]/11 >100*%eps) then pause,end
if abs(x1'*p+x1'*Q*x1/2-1/22)>100*%eps then pause,end
if abs(C*x1-b)>100*%eps then pause,end

//sparse 
x1=qpsolve(Q,p,sparse(C),b,[],[],me);
if or(x1-[8;3]/11 >100*%eps) then pause,end
if abs(x1'*p+x1'*Q*x1/2-1/22)>100*%eps then pause,end
if abs(C*x1-b)>100*%eps then pause,end

//third test (R)

Q=eye(3,3);p=-[0;5;0];
C=-[-4  2  0;-3  1 -2;0  0  1];b=-[-8;2;0];me=0;

x1=qpsolve(Q,p,C,b,[],[],me);
if or(x1-[0;5;0] >100*%eps) then pause,end
if abs(x1'*p+x1'*Q*x1/2+12.5)>100*%eps then pause,end
if or(abs(C*x1-b-[-18;-3;0])>100*%eps) then pause,end
//sparse
x1=qpsolve(Q,p,sparse(C),b,[],[],me);
if or(x1-[0;5;0] >100*%eps) then pause,end
if abs(x1'*p+x1'*Q*x1/2+12.5)>100*%eps then pause,end
if or(abs(C*x1-b-[-18;-3;0])>100*%eps) then pause,end

//fourth test (Matlab)
Q= [1 -1; -1 2] ;
p = [-2; -6];
C = [1 1; -1 2; 2 1];
b = [2; 2; 3];
lb = zeros(2,1);
me=0;
x1=qpsolve(Q,p,C,b,[],[],me);
if or(x1-[2;4]/3 >100*%eps) then pause,end
if abs(x1'*p+x1'*Q*x1/2+74/9)>100*%eps then pause,end
if or(abs(C*x1-b-[0;0;-1/3])>100*%eps) then pause,end
//sparse
x1=qpsolve(Q,p,sparse(C),b,[],[],me);
if or(x1-[2;4]/3 >100*%eps) then pause,end
if abs(x1'*p+x1'*Q*x1/2+74/9)>100*%eps then pause,end
if or(abs(C*x1-b-[0;0;-1/3])>100*%eps) then pause,end

//avec bornes
Q= [1 -1; -1 2] ;
p = [-2; -6];
C = [1 1; -1 2; 2 1];
b = [2; 2; 3];
lb = [0.8;0]; //zeros(2,1);
me=0;
x1=qpsolve(Q,p,C,b,lb,[],me);
if or(x1-[0.8;1.2] >100*%eps) then pause,end
if abs(x1'*p+x1'*Q*x1/2+8)>100*%eps then pause,end
if or(abs(C*x1-b-[0;-0.4;-0.2])>100*%eps) then pause,end
//sparse

x1=qpsolve(Q,p,sparse(C),b,lb,[],me);
if or(x1-[0.8;1.2] >100*%eps) then pause,end
if abs(x1'*p+x1'*Q*x1/2+8)>100*%eps then pause,end
if or(abs(C*x1-b-[0;-0.4;-0.2])>100*%eps) then pause,end

