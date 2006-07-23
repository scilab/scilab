function [k,wh]=ge_getobj(GraphList,pt)
//Copyright INRIA
//Author : Serge Steer 2002

// Copyright INRIA
  wh=[],
  k=ge_getnode(GraphList,pt)
  if k<>[] then wh='node',return,end
  k=ge_getarc(GraphList,pt)
  if k<>[] then wh='arc',return,end

endfunction
