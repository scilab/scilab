function [L,T]= francis(P22,F,G);
// Given the plant P22:
//    x'= A*x + B2*u 
//    y = C2*x + D22*u
// and the signal generator
//    w' = F*w 
//    y^ = G*w
// This macro returns the matrices L,T which satisfy the Francis equations
//    [A      B2]      [ T ]      [T*F]
//                  *         = 
//    [C2    D22]      [ L ]       [G]
//  T is nx x nw  and L is ny x nw
// If K stabilizes the pair (A,B2) (i.e. A+B2*K is Hurwitz)  then:
//    u = L*w + K*(x-T*w) = K*x + (L-K*T)*w stabilizes P22 
//    and y - y^ --> 0
// Copyright INRIA
[A,B2,C2,D22]=P22(2:5);
[n,mm]= size(B2);
[r,r1]=size(C2);
[d,d1]= size(F);
if r>mm then error('Francis: ny > nu!');end
Id=eye(d,d);
Mat=[Id.*.A-F'.*.eye(n,n), Id.*.B2;
     Id.*.C2   ,   Id.*.D22];
rhs=[0*ones(n*d,1);matrix(G,size(G,'*'),1)];
TL=Mat\rhs;
T=Tl(1:n*d);T=matrix(T,n,d);
L=TL(n*d+1:n*d+mm*d);L=matrix(L,mm,d);


