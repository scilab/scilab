function w=sdiff(z,d)
// time serie diiferencing 
// W= (1-B)^d Z 
// where B is the backward-shift operator 
// and d the difrenecing order 
// z : a matrix of size(n1,n2) z(t)=z(:,t)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1;d=1;end
w=z;
for i=1:d,[n1,n2]=size(w); w=w(:,2:n2)-w(:,1:(n2-1));end
endfunction
