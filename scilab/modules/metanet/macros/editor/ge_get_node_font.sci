
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function font=ge_get_node_font(n)
  if GraphList.nodes.graphics.font==[] then
    font=GraphList.nodes.graphics.defaults.font*ones(1,size(n,'*'))
  else
    font=GraphList.nodes.graphics.font(:,n)
    for k=1:3
      font(k,font(k,:)==0)=GraphList.nodes.graphics.defaults.font(k)
    end
  end
endfunction

