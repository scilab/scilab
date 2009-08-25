
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g=%c_i_graph(i,d,g)
//called for compatibility g.node_x, g.node_y g.edge_color
  if  i=='node_name' then
    g.nodes.graphics.name=d
  elseif i=='edge_label' then
    F=getfield(1,g.edges.data)
    if and(F<>'label') then 
      data=g.edges.data
      setfield(1,[F 'label'],data)
      data.label=emptystr(1,edge_number(g))
      g.edges.data=data
    end
    g.edges.data.label=d
  elseif i=='edge_weight' then
    F=getfield(1,g.edges.data)
    if and(F<>'weight') then 
      data=g.edges.data
      setfield(1,[F 'weight'],data)
      data.weight=zeros(1,edge_number(g))
      g.edges.data=data
    end
    g.edges.data.weight=d
  end
endfunction
