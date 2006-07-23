function ge_hilite_arcs(p,GraphList)
//Copyright INRIA
//Author : Serge Steer 2002
  b=GraphList.edge_width(p)
  b(b==0)=GraphList.default_edge_width
  b=3*b
  GraphList.edge_width(p)=b
  ge_drawarcs(p)
endfunction
