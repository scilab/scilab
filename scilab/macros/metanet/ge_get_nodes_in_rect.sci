function [in,out]=ge_get_nodes_in_rect(GraphList,rect)
//Copyright INRIA
//Author : Serge Steer 2002

// Copyright INRIA
  in=[];out=[]
  x=GraphList.node_x;
  y=GraphList.node_y;
  ox=rect(1);oy=rect(2),w=rect(3),h=rect(4)
  in=matrix(find( ((ox-x).*(ox+w-x))<0 & ((oy-y).*(oy-h-y))<0),1,-1)
  out=1:size(GraphList.node_x,'*'),out(in)=[];
endfunction
