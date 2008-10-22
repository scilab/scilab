//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

TCL_EvalFile(tkpath+'timer')
while %t //wait for toplevel to disapear
  TCL_EvalStr('set h [winfo exists .vscale]');
  if TCL_GetVar("h")=='0' then break,end
end
