function k=ge_getnode(GraphList,pt)
//Copyright INRIA
//Author : Serge Steer 2002

// Copyright INRIA
  x=pt(1);y=pt(2)
  d=GraphList.node_diam;d(d==0)=GraphList.default_node_diam;
  k=find(sqrt((GraphList.node_x-x)^2+(GraphList.node_y-y)^2) <= d/2)
  if k<>[] then k=k(1),end
endfunction
