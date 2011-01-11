// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA, Serge Steer
// Copyright (C) - 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function y = asind(x)
//Inverse sine, result in degrees.
  if ~isreal(x)
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"asind",1)); 
  end
  if min(x)<-1|max(x)>1 then
    error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the interval [%s, %s].\n"),"asind",1,"-1","1"));
  end
  y = 180/%pi*asin(x);
endfunction
