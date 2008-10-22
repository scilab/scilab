//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

tkpath = get_absolute_file_path("scroll.dem.sce");

TCL_EvalFile(tkpath+'cscroll')
while %t //wait for toplevel to disapear
  TCL_EvalStr('set h [winfo exists .cscroll]');
  if TCL_GetVar("h")=='0' then break,end
  sleep(1);
end
