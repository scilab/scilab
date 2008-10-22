// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// @OBSOLETE

function answ=buttondialog(msg,btn,icon)

//warnobsolete("messagebox", "5.1");

rhs = argn(2);

if rhs == 1 then
  error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), "buttondialog",2,3));
elseif rhs == 2 then
  answ = messagebox(msg, "modal", tokens(btn, "|"));
else
  answ = messagebox(msg, "modal", icon, tokens(btn, "|"));
end

answ = string(answ);

endfunction

