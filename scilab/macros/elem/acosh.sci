function t=acosh(x)
//Element wise Hyberbolic cosine inverse of x
//Entries of x in ]1,+inf[
//Outputs in   ]0,+inf[ x ]-pi,pi]
//                  and    [0] x [0,pi]
//!
// Copyright INRIA
if type(x)<>1 then error(53),end
[m,n]=size(x)
t=log(x+(x+ones(m,n)).*sqrt((x-ones(m,n))./(x+ones(m,n))))




