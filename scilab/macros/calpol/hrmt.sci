function [pg,U]=hrmt(v)
// Syntax: [pg,U]=hrmt(v)
// Finds unimodular U and pg = gcd of a row of polynomials v
// such that v*U = [pg,0]
//!
// Copyright INRIA
[n,m]=size(v)
if n>1 then error(60),end
pg=v(1)
U=eye(m,m)
for k=2:m
 [pg,uk]=bezout(pg,v(k))
 U(:,k-1:k)=U(:,k-1:k)*uk(:,[2 1])
end
endfunction
