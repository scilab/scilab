// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F.D
// Copyright (C) INRIA - 1996 - C. Bunks
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [pols,gain]=zpch1(n,epsilon,omegac)
    //Poles of a Type 1 Chebyshev analog filter
    //The transfer function is given by :
    //H(s)=gain/poly(pols,'s')
    //  n       :Filter order
    //  epsilon :Ripple in the pass band (0<epsilon<1)
    //  omegac  :Cut-off frequency in rd/s
    //  pols    :Resulting filter poles
    //  gain    :Resulting filter gain
    //
    //!
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
