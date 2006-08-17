function [y]=dsimul(sld,u)
// Copyright INRIA
[a,b,c,d,x0]=sld(2:6);
y=c*ltitr(a,b,u,x0)+d*u;
endfunction
