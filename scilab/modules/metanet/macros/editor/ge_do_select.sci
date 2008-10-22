
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [k_nodes,k_arcs]=ge_do_select(GraphList,xc,yc)

  ge_disablemenus()
  rect=[]
  [rect,btn]=rubberbox([xc,yc])
  if btn==2 then return,end

  [k_nodes,del]=ge_get_nodes_in_rect(GraphList,rect);
  k_arcs=[]
  if k_nodes<>[] then 
    na=size(GraphList.head,'*')
    if del==[] then 
      k_arcs=1:na
    elseif size(k_nodes,'*')>1&na>0 then  
      k_nodes=gsort(k_nodes,'g','i')
      k_arcs=find(dsearch(GraphList.tail,k_nodes,'d')<>0&dsearch(GraphList.head,k_nodes,'d')<>0)
    end
  end
  ge_hilite_nodes(k_nodes,GraphList)
  ge_hilite_arcs(k_arcs,GraphList)
  ge_enablemenus()
  
endfunction
