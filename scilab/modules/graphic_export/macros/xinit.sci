// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function  xinit(fileName)
  // set name of the file to export

  global %exportFileName;

  rhs = argn(2);

  global %driverName;
  if (%driverName == "Rec" | %driverName == "X11" | %driverName == "" | %driverName == []) then
    // screen display, should be xinit without argument
    if (rhs <> 0) then
      error(msprintf(gettext("%s: Wrong number of input arguments: Function has no input argument with ''%s'' or ''%s'' drivers.\n"), "xinit","Rec","X11"));
    end
    scf();
  else
    // file driver
    if (rhs <> 1) then
	  error(msprintf(gettext("%s: Wrong number of input argument: String expected with ''%s'', ''%s'', ''%s'' or ''%s'' drivers.\n"), "xinit","GIF","PPM","POS","Fig"));

    end

    if (type(fileName) <> 10) then
      // wrong type for driverName
      error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "xinit",1));
      return;
    end
  
    %exportFileName = fileName;
    
  end

endfunction


