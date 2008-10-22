function [reg,rect,keep]=ge_get_region(xc,yc,win)
//Copyright INRIA
//Author : Serge Steer 2002

// Copyright INRIA
  xset('window',win)
  reg=list();rect=[]
  [rect,btn]=rubberbox([xc,yc])
  if btn==2 then return,end

  [keep,del]=ge_get_nodes_in_rect(GraphList,rect);
  rect(2)=rect(2)-rect(4)

  if keep==[] then return,end
  if del==[] then reg=GraphList,return,end
  if size(keep,'*')>1 then  
    del_arcs=[];
    na=size(GraphList.head,'*')
    keep=gsort(keep,'g','i')
    k_arcs=find(dsearch(GraphList.tail,keep,'d')<>0&dsearch(GraphList.head,keep,'d')<>0)
    del_arcs=1:na;del_arcs(k_arcs)=[]
  else
    k_arcs=[]
  end

  reg=editgraph_diagram()

  for f=ge_node_fields(),
    reg(f)=[GraphList(f)(keep)];
  end
  if k_arcs<>[] then
    for f=ge_arc_fields(),
      reg(f)=[GraphList(f)(k_arcs)];
    end
    //renumber
    k1=1:size(reg.node_x,'*')
    reg.tail=k1(dsearch(reg.tail,keep,'d'))
    reg.head=k1(dsearch(reg.head,keep,'d'))
  end
endfunction
