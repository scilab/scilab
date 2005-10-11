function [ro,teta]=polar(a);
//[ro,Teta]=polar(A);
// Polar form of A
// A=Ro*exp(%i*Teta) Ro symmetric >=0 
// Teta hermitian >=0
//F.D.
//!
// Copyright INRIA
  [U,s,V]=svd(a);
  ro1=U*sqrt(s);
  ro=ro1*ro1';
  W=U*V';
  // A = Ro*W   (Ro sdp ; W unit)
  // W=exp(%i*Teta)
  //
  [ab,x,bs]=bdiag(W+0*%i*W);
  z=log(diag(ab));
  lw=x*diag(z)*inv(x);
  teta=-%i*lw;
endfunction
