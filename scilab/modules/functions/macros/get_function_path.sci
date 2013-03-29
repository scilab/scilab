// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2009-2010 - Allan CORNET
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function path = get_function_path(name)

  path = [];
  rhs = argn(2);
  
  if rhs == 0 then
      error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "get_function_path", 1));
  end

  if type(name) <> 10 then
    error(999,msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"),"get_function_path",1));
  end

  if size(name,"*") <> 1 then
    error(999,msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"get_function_path",1));
  end

  libname = whereis(name);

  if libname <> [] then
    for i = 1:size(libname,"*")
      [funcnames, pathlib] = libraryinfo(libname(i));
      path = [path ; pathlib + name + ".sci"];
    end
    path = pathconvert(path,%F);
  end

endfunction
