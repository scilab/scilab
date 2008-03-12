// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// @OBSOLETE

function varargout = x_message_modeless(msg, buttons)

//warnobsolete("messagebox", "5.1");

rhs = argn(2);

if rhs==1
  messagebox(msg, "non-modal");
  varargout(1) = [];
else
  answ = messagebox(msg, "non-modal", buttons);
  varargout(1) = answ;
end

endfunction
