function [k_nodes,k_arcs]=ge_do_select(GraphList,xc,yc)
//Copyright INRIA
//Author : Serge Steer 2002
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
