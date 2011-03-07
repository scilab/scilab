// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA, Serge Steer
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function y = tand(x)
// Tangent of x in degrees.
// tand(x) = tan(x*%pi/180) in the mathematical sense
// But floating point numbers behave differently.
// For example, tand(180) = 0 while tan(pi) = -1.225D-16
  if ( ~isreal(x) ) then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"tand",1)); 
  end
  // Argument reduction toward [-90,90[
  n = round(x/180)
  x = x - n*180
  y = tan(%pi/180*x)
  // Set all singular points to nan
  m = pmodulo(x+90,180)
  y(m==0) = %nan
endfunction

