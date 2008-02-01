function [rect]=ge_dig_bound(GraphList)
//Copyright INRIA
//Author : Serge Steer 2002

// Copyright INRIA

  d=GraphList.nodes.graphics.diam
  
  xmin=min(GraphList.nodes.graphics.x-d/2)
  xmax=max(GraphList.nodes.graphics.x+d/2)

  k1=find(GraphList.nodes.graphics.type==0|GraphList.nodes.graphics.type==3)
  ymin=min(GraphList.nodes.graphics.y(k1)-d(k1)/2)
  ymax=max(GraphList.nodes.graphics.y(k1)+d(k1)/2)
  
  k1=find(GraphList.nodes.graphics.type==1) //sinks
  if k1<>[] then
    ymin=min(ymin,min(GraphList.nodes.graphics.y(k1)-d(k1)/2)-2.5)
    ymax=max(ymax,max(GraphList.nodes.graphics.y(k1)+d(k1)/2))
  end
  k1=find(GraphList.nodes.graphics.type==1) //sources
  if k1<>[] then
    ymin=min(ymin,min(GraphList.nodes.graphics.y(k1)-d(k1)/2))
    ymax=max(ymax,max(GraphList.nodes.graphics.y(k1)+d(k1)/2)+2.5)
  end
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
