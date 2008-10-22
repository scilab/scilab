
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [rect]=ge_dig_bound(GraphList)

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
