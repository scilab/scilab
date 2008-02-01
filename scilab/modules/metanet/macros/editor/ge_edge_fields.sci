function [fa,fd]=ge_edge_fields()
//Copyright INRIA
//Author : Serge Steer 2002
  F=getfield(1,GraphList.edges)
  fa = F(2:$) //skip the type, the default field and the data field
  F=getfield(1,GraphList.edges.data)
  fd = F(2:$);
endfunction
