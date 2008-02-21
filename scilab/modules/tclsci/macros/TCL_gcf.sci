// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// @OBSOLETE

function f = TCL_gcf()
// Get current graphic window (this function is obsolete since Uicontrols are Java objects)

warnobsolete("gcf", "5.1");

[lhs,rhs]=argn();

if rhs > 0 then
  error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "TCL_gcf", 0));
end

if lhs > 1 then
  error(msprintf(gettext("%s: Wrong number of output argument: %d to %d expected.\n"), "TCL_gcf", 0, 1));
end

f = gcf();
endfunction

