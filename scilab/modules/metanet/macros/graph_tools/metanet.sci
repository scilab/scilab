
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function scr=metanet(path,winsize)
  disp("""metanet"" is obsolete. Use ""edit_graph"" instead")
  if argn(2)<1 then
    scr=edit_graph() 
  elseif argn(2)==1 then
    scr=edit_graph(path) 
  else
    scr=edit_graph(path,1,winsize) 
  end
endfunction
