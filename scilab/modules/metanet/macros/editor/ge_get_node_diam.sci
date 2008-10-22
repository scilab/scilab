
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function diam=ge_get_node_diam(n)
  if GraphList.nodes.graphics.diam==[] then
    diam=GraphList.nodes.graphics.defaults.diam(1,ones(n))
  else
    diam=GraphList.nodes.graphics.diam(n)
    diam(diam==0)=GraphList.nodes.graphics.defaults.diam
  end
endfunction
