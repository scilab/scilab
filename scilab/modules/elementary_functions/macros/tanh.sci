// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [t]=tanh(z)
// 
//  PURPOSE 
//    element wise hyperbolic tangent
//
//  METHOD 
//    based on the formula  tanh(z) = -i tan(i z)
//
  if type(z)<>1 then error(53,1),end
  if isreal(z) then
     t = imag(tan(imult(z)))
  else
     t = -imult(tan(imult(z)))
  end
endfunction
