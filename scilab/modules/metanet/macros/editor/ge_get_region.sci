
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [reg,rect,keep]=ge_get_region(xc,yc,win)
  reg=list();rect=[]
  [rect,btn]=rubberbox([xc,yc])
  if btn==2 then return,end

  [keep,del]=ge_get_nodes_in_rect(GraphList,rect);
  rect(2)=rect(2)-rect(4)

  if keep==[] then return,end
  if del==[] then reg=GraphList,return,end
  if size(keep,'*')>1 then  
    del_arcs=[];
    na=size(GraphList.edges)
    keep=gsort(keep,'g','i')
    k_arcs=find(dsearch(GraphList.edges.tail,keep,'d')>0&dsearch(GraphList.edges.head,keep,'d')>0)
    del_arcs=1:na;del_arcs(k_arcs)=[]
  else
    k_arcs=[]
  end

  reg=ge_new_graph()
  reg.nodes=GraphList.nodes(keep)
  reg.edges=GraphList.edges(k_arcs)
  if k_arcs<>[] then
    //renumber
    k1=(1:size(reg.nodes))
    reg.edges.tail=k1(dsearch(reg.edges.tail,keep,'d'))
    reg.edges.head=k1(dsearch(reg.edges.head,keep,'d'))
  end
endfunction
