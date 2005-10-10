function t=coth(x)
//Syntax : t=coth(x)
//
// hyperbolic co-tangent of x
//!
// Copyright INRIA
  if type(x)<>1 then error(53,1),end
  t=exp(x);
  t=(t-ones(x)./t).\(t+ones(x)./t)
endfunction
