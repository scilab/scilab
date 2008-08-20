// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function status = TCL_CreateSlave(slaveName)

// Input arguments tests
[lhs, rhs] = argn();
if rhs<>1 then
  error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "TCL_CreateSlave", 1));
end

if lhs<>1 then
  error(msprintf(gettext("%s: Wrong number of output argument(s): %d expected.\n"), "TCL_CreateSlave", 1));
end


if typeof(slaveName)<>"string" then
  error(msprintf(gettext("%s: Wrong type for input argument #%d: A String expected.\n"), "TCL_CreateSlave", 1));
end

if TCL_ExistInterp(slaveName) then
  error(msprintf(gettext("%s: Slave interpreter already exists.\n"),"TCL_CreateSlave"));
end

status = TCL_EvalStr("interp create "+slaveName);

endfunction
