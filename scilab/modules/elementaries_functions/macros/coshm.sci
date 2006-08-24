function x=coshm(a)
// hyperbolic cosine of square matrix x
//!
// Copyright INRIA
  if type(a)<>1 then error(53,1),end
  if a==[] then x=[],return,end
  x=(expm(a)+expm(-a))/2
endfunction
