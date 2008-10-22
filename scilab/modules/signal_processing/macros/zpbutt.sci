// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F.D
// Copyright (C) INRIA - 1996 - C. Bunks
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [pols,gain]=zpbutt(n,omegac)
//<pols,gain>=zpbutt(n,omegac)
//Computes the poles of a Butterworth analog
//filter of order n and cutoff frequency omegac
//transfer function H(s) is calculated by
//     H(s) = gain/real(poly(pols,'s'))
//  n      :Filter order
//  omegac :Cut-off frequency in Hertz
//  pols   :Resulting poles of filter
//  gain   :Resulting gain of filter
//
//! 
angles=ones(1,n)*(%pi/2+%pi/(2*n))+(0:n-1)*%pi/n;
pols=omegac*exp(%i*angles);
gain=abs((-omegac)^n);
endfunction
