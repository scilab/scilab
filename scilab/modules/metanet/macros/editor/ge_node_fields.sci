function [fn,fd]=ge_node_fields(nodes)
//Copyright INRIA
//Author : Serge Steer 2002
  if argn(2)<1 then nodes=GraphList.nodes;end
  F=getfield(1,nodes)
  fn = F(2:$) //skip the type,
  F=getfield(1,nodes.data)
  fd = F(2:$);
endfunction
