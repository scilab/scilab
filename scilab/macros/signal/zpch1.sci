function [pols,gain]=zpch1(n,epsilon,omegac)
//Poles of a Type 1 Chebyshev analog filter
//The transfer function is given by :
//H(s)=gain/poly(pols,'s')
//  n       :Filter order
//  epsilon :Ripple in the pass band (0<epsilon<1)
//  omegac  :Cut-off frequency in Hertz
//  pols    :Resulting filter poles
//  gain    :Resulting filter gain
//
//!
//Author F.D.
//Revised by C. Bunks Oct. 24, 1996  
// Copyright INRIA
Gamma=((1+sqrt(1+epsilon**2))/epsilon)^(1/n);
a=omegac*(Gamma-1/Gamma)/2;
b=omegac*(Gamma+1/Gamma)/2;
v=%pi/(2*n):%pi/n:(2*n-1)/(2*n)*%pi;
sigma=-a*sin(v);
omega=b*cos(v);
pols=sigma+%i*omega;
gain=abs(real(prod(pols)));
if n==2*int(n/2) then,
      gain=gain/sqrt(1+epsilon^2);
end
    
endfunction
