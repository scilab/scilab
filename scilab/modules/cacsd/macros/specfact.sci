function [w0,l]=specfact(a,b,c,d)
// Copyright INRIA
r=d+d';w0=sqrtm(r);
p=ricc(a-b/r*c,-b/r*b',c'/r*c,'cont'); 
//a'*p+p*a+(c'-p*b)*inv(r)*(c-b'*p) is zero
l=w0\(c-b'*p)
endfunction
