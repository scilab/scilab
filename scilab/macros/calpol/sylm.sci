function [s]=sylm(a,b)
//[s]=sylm(a,b) gives the Sylvester matrix associated to polynomials
//a and b, i.e. the matrix s such that:
//  coeff( a*x + b*y )' = s * [coeff(x)';coeff(y)']
//dimension of s is equal to degree(a)+degree(b)
//If a and b are coprime polynomials
//(rank(sylm(a,b))=degree(a)+degree(b)) the instructions
//  u = sylm(a,b) \ eye(na+nb,1)
//  x = poly(u(1:nb),'z','coeff')
//  y = poly(u(nb+1:na+nb),'z','coeff')
//compute Bezout factors x et y of minimal degree de degre minimal
//such that a*x+b*y=1
//!
// Copyright INRIA
na=degree(a);a=coeff(a)';
nb=degree(b);b=coeff(b)';
s(na+nb,na+nb)=0;
for i=1:nb,s(i:na+i,i)=a,end
for i=1:na,s(i:nb+i,nb+i)=b,end
endfunction
