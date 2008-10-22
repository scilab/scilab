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

  rhs = argn(2);

  if rhs <> 1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"tanh",1));
  end

  if type(z)<>1 then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Real or complex matrix expected.\n"),"sinhm",1));
  end

  if isreal(z) then
    t = imag(tan(imult(z)));
  else
    t = -imult(tan(imult(z)));
  end

endfunction
