function [rect]=ge_dig_bound(GraphList)
//Copyright INRIA
//Author : Serge Steer 2002

// Copyright INRIA
  xmin=min(GraphList.node_x)
  xmax=max(GraphList.node_x)
  ymin=min(GraphList.node_y)
  ymax=max(GraphList.node_y)
  rect=[xmin,ymin,xmax,ymax]
endfunction
