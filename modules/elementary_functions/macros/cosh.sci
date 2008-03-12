// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [t] = cosh(z)
  // 
  //  PURPOSE 
  //     element wise hyperbolic cosinus
  //
  //  METHOD 
  //     1/ in the real case use
  //
  //         cosh(z) = 0.5 (exp(|z|) + exp(-|z|))
  //                 = 0.5 ( y + 1/y ) with y = exp(|z|)
  //
  //         The absolute value avoids the problem of a 
  //         division by zero arising with the formula  
  //           "cosh(z) = 0.5 ( y + 1/y ), y=exp(z)"
  //         when ieee = 0 for z such that exp(z) equal 0 
  //         in floating point arithmetic ( approximatly z < -745 ) 
  //
  //     2/ in the complex case use :   cosh(z) = cos(i z)
  //
  if type(z)<>1 then error(53,1),end
  if isreal(z) then
    y = exp(abs(z)) ; t = 0.5*(y + 1 ./y)
  else
    t = cos(imult(z))
  end
endfunction
