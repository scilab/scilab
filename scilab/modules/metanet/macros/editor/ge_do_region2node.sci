
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function GraphList=ge_do_region2node(GraphList,xc,yc)
  ge_disablemenus()
  [rect,btn]=ge_get_rectangle([xc,yc])
  if btn==2 then  ge_enablemenus();return;end
  [del,keep]=ge_get_nodes_in_rect(GraphList,rect);
  
  modified=del<>[]
  if ~modified then return,end
  
  
  [in,out]=ge_get_nodes_in_rect(GraphList,rect);
  if in==[] then return,end
  GraphList_save=GraphList

  del_arcs=[];//arcs completly in in
  for k=1:size(GraphList.head,'*')
    if or(GraphList.tail(k)==in)&or(GraphList.head(k)==in) then
      del_arcs=[del_arcs,k];
    end
  end
  ox=rect(1);oy=rect(2),w=rect(3);h=rect(4)
  ge_drawobjs(GraphList,in,del_arcs) //erase
  GraphList=ge_delete_arc(GraphList,del_arcs)
  GraphList=ge_delete_node(GraphList,in)
  GraphList=ge_add_node(GraphList,ox+w/2,oy-h/2)
  n=size(GraphList.node_x,'*')

  // arcs passing through the rectangle
  mod_arcs=[]
  for k=1:size(GraphList.head,'*')
    if or(GraphList.tail(k)==in) then
      GraphList.tail(k)=n
      mod_arcs=[mod_arcs k]
    elseif or(GraphList.head(k)==in) then
      GraphList.head(k)=n
      mod_arcs=[mod_arcs k]
    end
  end
  ge_drawobjs(GraphList,n,mod_arcs) //draw new node and arcs

  edited=resume(%t)
endfunction
