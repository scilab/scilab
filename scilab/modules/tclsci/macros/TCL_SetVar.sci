// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function status = TCL_SetVar(varName, value, slaveName)

// Input arguments tests
[lhs, rhs] = argn();
if rhs<>2 & rhs<>3 then
  error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "TCL_SetVar", 2, 3));
end

if lhs<>1 then
  error(msprintf(gettext("%s: Wrong number of output argument(s): %d expected.\n"), "TCL_SetVar", 1));
end

if typeof(varName)<>"string" then
  error(msprintf(gettext("%s: Wrong type for input argument #%d: A String expected.\n"), "TCL_SetVar", 1));
end

isComplex = %F;
if typeof(value)=="constant" then
  isComplex = ~isreal(value);
  value = string(value);
end

if typeof(value)<>"string" | isComplex then
  error(msprintf(gettext("%s: Wrong type for input argument #%d: Real or String matrix expected.\n"), "TCL_SetVar", 2));
end

if rhs==3 then
 if typeof(slaveName)<>"string" then
  error(msprintf(gettext("%s: Wrong type for input argument #%d: A String expected.\n"), "TCL_SetVar", 3));
  return
 end
 if ~TCL_ExistInterp(slaveName) then
    error(msprintf(gettext("%s: No such slave interpreter.\n"),"TCL_SetVar"));
    return
  end
end

if size(value, "*") == 1 then // Scalar value
  if rhs==3 then
    command = "interp eval "+slaveName+" {set "+varName+" {"+value+"}}";
    disp(command)
    status = TCL_EvalStr(command);
  else
    status = TCL_EvalStr("set "+varName+" {"+value+"}");
  end
else // Matrix
  for nbRow=1:size(value, 1)
    for nbCol=1:size(value, 2)
      if rhs==3 then
	status = TCL_EvalStr("interp eval "+slaveName+" {set "+varName+"("+string(nbRow)+","+string(nbCol)+") {"+value+"}}");
      else
	status = TCL_EvalStr("set "+varName+"("+string(nbRow)+","+string(nbCol)+") {"+value+"}");
      end
    end
  end
end

endfunction
