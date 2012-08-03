// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [f]=scf(h)
//set default figure, h may be a handle or a figure_id
// (small correction to set up win number 0 when no
//  gr window is opened, Bruno 22 nov 2004)
  if argn(2)<1 then
     num=winsid()
     if num == [] then
	h = 0
     else
	h = max(num)+1
     end
  end
  set("current_figure",h);
  f=get("current_figure");
endfunction
