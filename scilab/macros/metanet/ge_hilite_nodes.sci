function ge_hilite_nodes(p,GraphList)
//Copyright INRIA
//Author : Serge Steer 2002

  b=GraphList.node_border(p)
  b(b==0)=GraphList.default_node_border
  b=3*b
  GraphList.node_border(p)=b
  ge_drawnodes(p)
  
endfunction
