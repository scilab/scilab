// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is released into the public domain

tkpath = get_absolute_file_path("puzzle.dem.sce");

TCL_EvalFile(tkpath+'puzzle')
while %t //wait for toplevel to disapear
  order=TCL_GetVar('order');
  TCL_EvalStr('set h [winfo exists .puzzle]');
  if TCL_GetVar("h")=='0' then break,end
  sleep(1);
end
disp(order)
