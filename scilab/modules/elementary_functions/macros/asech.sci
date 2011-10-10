// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA, Serge Steer
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function y = asech(x)

  //Inverse hyperbolic secant
  //asech(x)=  arccosh(1 ./ x)= i*csgn(i*(1−1 ./ x))*asec(x)=csgn(i*(1 − 1 ./ x))*(%pi/2*(i+acsch(i*x)))

  rhs = argn(2);
  if rhs <> 1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"asech", 1));
  end
  
  y = acosh(ones(x)./x);
endfunction
