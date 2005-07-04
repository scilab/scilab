function [rect]=ge_dig_bound(GraphList)
//Copyright INRIA
//Author : Serge Steer 2002

// Copyright INRIA
  xmin=min(GraphList.node_x)
  xmax=max(GraphList.node_x)
  ymin=min(GraphList.node_y)
  ymax=max(GraphList.node_y)
  if xmin==xmax then
    xmin=xmin-300;
    xmax=xmax+300;
  end
  if ymin==ymax then
    ymin=ymin-200;
    ymax=ymax+200;
  end
  rect=[xmin,ymin,xmax,ymax]
endfunction
