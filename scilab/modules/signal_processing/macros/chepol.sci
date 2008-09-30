// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F.D
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function Tn=chepol(n,var)
//Chebychev polynomial
//  n   :Polynomial order
//  var :Polynomial variable (character string)
//  Tn  :Polynomial in var
//
//!
if n==0 then
  Tn=poly(1,var,'coeff'),
elseif n==1 then
  Tn=poly(0,var);
else
  T0=poly(1,var,'coeff');
  T1=poly(0,var)
  for k=2:n
    Tn=2*poly(0,var)*T1-T0
    [T1,T0]=(Tn,T1);
  end
end
    
endfunction
