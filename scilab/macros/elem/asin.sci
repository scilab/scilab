function t=asin(x)
//Eelemt wize Sine-inverse of x
//Entries of vector x must be in [-1,+1]
//Entries of t are in    ]-pi/2,pi/2[ x ]-inf,+inf[
//                   -pi/2 x [0,+inf] and pi/2 x ]-inf,0] (real x imag)
//
//!
// Copyright INRIA
if type(x)<>1 then error(53),end
if x==[] then t=[],return,end
t=-%i*log(%i*x+sqrt(ones(x)-x.*x))
if norm(imag(x))==0&maxi(abs(x))<=1 then  t=real(t);end



