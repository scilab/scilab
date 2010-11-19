// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007 - INRIA - Pierre MARECHAL
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// isnum(str) return %T if str represent a number
//

function res = isnum(str)

  // Check input argument

  if argn(2) <> 1 then
    error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "isnum", 1));
  end

  if type(str) <> 10 then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: A string array expected.\n"), "isnum", 1));
  end

  // bug 8206 isnum did not manage blanks
  str = stripblanks(str);
  
  // Loop on input argument entries
  for i=1:size(str, "*") do
    res(i) = %F;
    if execstr("v = evstr(str(i))" , "errcatch") == 0 then
      if type(v) == 1 & v <> [] then
        res(i) = %T;
      end
    end
  end

  // Reshape the resulting matrix
  res = matrix(res, size(str));

endfunction
