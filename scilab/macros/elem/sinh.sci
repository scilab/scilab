function t=sinh(x)
//Syntax : t=sinh(x)
//
//element wise hyperbolic sine 
//!
// Copyright INRIA
if type(x)<>1 then error(53),end
t=exp(x)
t=(t-ones(t)./t)/2


