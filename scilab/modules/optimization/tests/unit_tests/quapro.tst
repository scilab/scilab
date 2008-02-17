// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU <sylvestre.ledru@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

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
//with  x between ci and cs:
ci=[-1000;-10000;0;-1000;-1000;-1000];cs=[10000;100;1.5;100;100;1000];
//and minimize 0.5*x'*Q*x + p'*x with
p=[1;2;3;4;5;6]; Q=eye(6,6);
//No initial point is given;
C=[C1;C2] ; //
b=[b1;b2] ;  //
me=3;
[x,lagr,f]=quapro(Q,p,C,b,ci,cs,me)
//Only linear constraints (1 to 4) are active (lagr(1:6)=0):
[x,lagr,f]=quapro(Q,p,C,b,[],[],me)   //Same result as above


// Example retrieved from: 
// http://www.linbox.com/ucome.rvt/any/doc_distrib/scilab-2.6/enpc/Berthaud_reponses

r=1; k=400; m=1; g=10;
Q=1/2*k*[1,-1,0,0;-1,2,-1,0;0,-1,2,-1;0,0,-1,1];
P=m*g*r*[3;2;1;0];

C=[0,0,0,1]; b=0; m=1;
alphai=[];alphau=[];

[alpha,lag,Umin]=quapro(Q,P,C,b,alphai,alphau,m)

// Example retrieved from:
// http://matman.uwm.edu.pl/~kulesza/Symulacje_komputerowe/SciLab/scilab12.pdf

Q = [2,-1; -1,3]; c=[2;4]; C=[-2, -3; -5, -1]; d=[-5; -1]

XL = [0; 0]; XU = [1e10;1e10];

[xopt, larg, fopt] = quapro(Q, c, C, d, XL, XU)

// Example retrieved from:
// http://www.poli.usp.br/d/pmr5215/Introscilab.pdf
H = [ 2 0; 0 2];
c = [-6; -8];
A = [ 1 2; 3 1];
b = [ 3 ; 4];
[x, lagr, qx ] = quapro(H, c, A, b, zeros(2,1), [])
