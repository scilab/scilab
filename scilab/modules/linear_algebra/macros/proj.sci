function [y]=proj(x1,x2)
//[y]=proj(x1,x2)  projection on x2 parallel to x1
//F.D.
//!
// Copyright INRIA
  [l,k]=size(x1);
  [w,n]=rowcomp(x1);w1=w(:,1:n);
  x1t=w(n+1:l,:);
  y=x2/(x1t*x2)*x1t
endfunction
