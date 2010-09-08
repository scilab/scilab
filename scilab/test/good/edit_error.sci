//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

function answ = edit_error(clearerror)
  if (argn(2) == 0) then 
    clearerror = %f;
  end
  [str, n, line, func] = lasterror(clearerror);
  if (n > 0) then
    if (func == "") then
      answ = gettext('the last error did not occur inside a library function.');
    else
      p = get_function_path(func);
      if (p <> []) then
        editor(p);
        // TODO: add command to hilite line
        answ = gettext('opening file ') + p + gettext(' on line ') + ..
                 msprintf("%d",line) + gettext(' of the source file of the function ') + func;
      else
        answ = gettext('the source file of the function ') + func + ..
                 gettext(' was not found in any library path');
      end
    end
  else
    answ = gettext(' was not found in any library path');
  end
endfunction

