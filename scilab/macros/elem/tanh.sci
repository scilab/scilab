function t=tanh(x)
//Element wise Hyperbolic tangent
// Copyright INRIA
if type(x)<>1 then error(53),end
t1=exp(x);t2=exp(-x);
t=(t1-t2)./(t1+t2);
k=find(isnan(t))
t(k)=sign(real(x(k)))
