
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function font=ge_get_edge_font(n)
  if GraphList.edges.graphics.font==[] then
    font=GraphList.edges.graphics.defaults.font
  else
    font=GraphList.edges.graphics.font(:,n)
    font(font==0)=GraphList.edges.graphics.defaults.font(font==0)
  end
endfunction
