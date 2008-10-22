
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function thick=ge_get_node_border(n)
  if GraphList.nodes.graphics.border==[] then
    thick=GraphList.nodes.graphics.defaults.border(1,ones(n))
  else
    thick=GraphList.nodes.graphics.border(n)
    thick(thick==0)=GraphList.nodes.graphics.defaults.border;
  end
endfunction
