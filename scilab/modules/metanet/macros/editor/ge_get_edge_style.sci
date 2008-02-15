
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [thick,col,style]=ge_get_edge_style(n)
  if GraphList.edges.graphics.width==[] then
    thick=GraphList.edges.graphics.defaults.width
  else
    thick=GraphList.edges.graphics.width(n)
    if thick==0 then thick=GraphList.edges.graphics.defaults.width,end
  end
  if GraphList.edges.graphics.foreground==[] then
    col=GraphList.edges.graphics.defaults.foreground
  else
    col=GraphList.edges.graphics.foreground(n)
    if col==0 then col=GraphList.edges.graphics.defaults.foreground,end
  end
  style=0
endfunction
