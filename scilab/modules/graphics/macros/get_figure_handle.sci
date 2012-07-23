// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [fig]=get_figure_handle(index)

  if argn(2)<>1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "get_figure_handle", 1));
  end

  // check index
  figureIndices = winsid();
  // check that index is within the opened windows
  if (find(figureIndices == index) == []) then
    fig = [];
    return;
  end

  // return the figure handle from its index
  curFig = gcf();
  fig = scf(index);
  
  // scf sets the figure, so switch back to curFig
  scf(curFig);

endfunction

