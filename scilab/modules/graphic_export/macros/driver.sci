// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function  [curDriver] = driver(driverName)
  // set or get the current driver

  global %driverName;

  if (argn(2) < 1) then
    // no param just return the current driver
    curDriver = %driverName;
  elseif (argn(2) == 1) then
    if (type(driverName) <> 10) then
      // wrong type for driverName
      error(55, 1);
      return;
    end
    
    if (~checkDriverName(driverName)) then
      error(gettext("driver: wrong value for first input argument: must be ""Rec"", ""X11"", ""Pos"", ""PPM"" or ""GIF""."));
     return;
    end
    
    %driverName = driverName;
    curDriver = driverName;

  else
    // incorrect number of arguments
    error(39);
    return;
  end


endfunction

function [check] = checkDriverName(driverName)
// check that the driver name is obne of the admissible ones
check =   driverName == "Rec"..
        | driverName == "X11"..
        | driverName == "Pos"..
        | driverName == "GIF"..
        | driverName == "PPM");

endfunction

