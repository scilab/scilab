
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [fn,fd]=ge_node_fields(nodes)
  if argn(2)<1 then nodes=GraphList.nodes;end
  F=getfield(1,nodes)
  fn = F(2:$) //skip the type,
  F=getfield(1,nodes.data)
  fd = F(2:$);
endfunction
