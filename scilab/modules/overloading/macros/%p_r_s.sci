// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function f=%p_r_s(p,m)
// f=%p_r_s(p,m) <=> f=p/m 
// with p matrix of polynomials, m constant matrix
//!

[l,c]=size(m)
[mp,np]=size(p);
if l==c then 
  f=p*inv(m)
else
  s=poly(0,varn(p))
  f=coeff(p,0)/m
  for k=1:maxi(degree(p))
    f=f+(coeff(p,k)/m)*(s^k)
  end
end
endfunction
