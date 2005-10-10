function x=sinhm(a)
//square matrix hyperbolic sine 
//!
// Copyright INRIA
  if type(a)<>1 then error(53,1),end
  if a==[] then x=[],return,end
  x=(expm(a)-expm(-a))/2
endfunction
