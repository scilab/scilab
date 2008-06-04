//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

tkpath = get_absolute_file_path("scale.dem.sce");

getf(tkpath+'demredraw.sci')

xbasc();set figure_style new;plot3d();done=%t
TCL_EvalFile(tkpath+'vscale')
while %t //wait for toplevel to disapear
  TCL_EvalStr('set h [winfo exists .vscale]');
  if TCL_GetVar("h")=='0' then break,end
  sleep(1)
end
