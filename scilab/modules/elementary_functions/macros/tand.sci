// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA, Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function y = tand(x)
// Tangent of x in degrees.
  if ~isreal(x) then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of reals expected.\n"),"tand",1)); 
  end
  
  n = round(x/90);
  x = x - n*90;
  z = (x == 0);
  m = pmodulo(n,2);
  y = x
  y(m==0) = tan(%pi/180*x(m==0));
  y(m==1 & ~z) = -1 ./ tan(%pi/180*x(m==1 & ~z));
  y(m==1 & z & n>=0) = 1/0;
  y(m==1 & z & n<0) = -1/0;
endfunction
