// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Close a graphic figure or delete an uimenu or an uicontrol
// This function has been written for compatibility with old TK uicontrols/uimenus

// @OBSOLETE

function f = TCL_scf(num)

warnobsolete("scf", "5.1");

[lhs,rhs]=argn();

if rhs <> 1 then
  error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "TCL_scf", 1));
end

if size(num, "*")<>1 | type(num)<>1 then
  error(msprintf(gettext("%s: Wrong type for input argument #%d: Single double value expected.\n"), "TCL_scf",1));
end

if lhs > 1 then
  error(msprintf(gettext("%s: Wrong number of output argument: %d to %d expected.\n"), "TCL_scf", 0, 1));
end

f = scf(num);
endfunction

