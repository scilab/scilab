function [m]=find_freq(epsilon,A,n)
//Search for m such that n=K(1-m1)K(m)/(K(m1)K(1-m))
//with m1=(epsilon*epsilon)/(A*A-1);
//If  m = omegar^2/omegac^2,the parameters
//epsilon,A,omegac,omegar and n are then
//compatible for defining a prototype elliptic filter.
//  epsilon :Passband ripple
//  A       :Stopband attenuation
//  n       :filter order
//  m       :Frequency needed for construction of
//          :elliptic filter
//
//!
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F.D
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

   m1=(epsilon*epsilon)/(A*A-1);
   chi1=%k(1-m1)/%k(m1);
   m=findm(chi1/n);
 
endfunction
