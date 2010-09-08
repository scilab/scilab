// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA, Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function z = atand(x,y)
//Inverse tangent, result in degrees.
  if ~isreal(x)
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of reals expected.\n"),"atand",1)); 
  end
  if argn(2)==2 then
    if ~isreal(y)
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of reals expected.\n"),"atand",2)); 
    end
    z = 180/%pi*atan(x,y);
  else
    z = 180/%pi*atan(x);
  end
endfunction

