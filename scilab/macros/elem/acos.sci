function t=acos(x)
// Element wise Arccosine Cosine-inverse
// Entries of vector x must be in [-1, 1]
// Entries of T are in   ] 0,pi[ x ]-inf +inf[
//                 [0] x [0,+inf] et [pi] x ]-inf,0]     (real x imag)
//!
// Copyright INRIA
if type(x)<>1 then error(53),end
if x==[] then t=[],return,end
t=-%i*log(x+%i*sqrt(ones(x)-x.*x))
if norm(imag(x))==0&maxi(abs(x))<=1 then 
  t=real(t);
end
endfunction
