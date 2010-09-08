// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA, Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function y = asecd(x)
//Inverse secant, result in degrees.
  if ~isreal(x) then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of reals expected.\n"),"asecd",1));
  end
  y = 180/%pi*acos(ones(x)./x);
endfunction
