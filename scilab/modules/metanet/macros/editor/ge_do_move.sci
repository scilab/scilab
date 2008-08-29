
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function GraphList=ge_do_move(GraphList,xc,yc)

// get a scicos object to move, and move it with connected objects
//!
  
//get block to move
  ge_disablemenus()
  ksel=ge_getnode([xc;yc])
  edited=ksel<>[]
  if ~edited then return,end
  
  ge_axes_handle=gca();n=size(ge_axes_handle.children,'*')
  gindex=ge_axes_handle.user_data;
  hedges=gindex.edge;hnodes=gindex.node;

  //get arcs connected to this node
  karcs=find(GraphList.edges.tail==ksel|GraphList.edges.head==ksel)
  loops=find(GraphList.edges.tail(karcs)==GraphList.edges.head(karcs))
  kloops=karcs(loops)
  karcs(loops)=[]
  //selected node and loop arcs will be simply moved whilst other arcs
  //need to be recomputed
  moved=[hnodes(ksel);hedges(kloops)]
  nmoved=size(moved,'*')
  modified=hedges(karcs)
  nmodified=size(modified,'*')
  
  x_save=GraphList.nodes.graphics.x(ksel);
  y_save=GraphList.nodes.graphics.y(ksel);
  
  ge_win_handle=gcf()
  ge_win_handle.immediate_drawing='off'
  ge_win_handle.pixel_drawing_mode='equiv'

  rep(3)=-1
  xy=[xc,yc]
  while rep(3)==-1 ,  // move loop
    rep=xgetmouse([%t %t])
    dxy=rep(1:2)-xy
    // erase current position
    for k=1:nmoved,draw(moved(k)),end //erase
    for k=1:nmodified,draw(modified(k)),end //erase
    
    for k=1:nmoved,move(moved(k),dxy),end
    GraphList.nodes.graphics.x(ksel)=GraphList.nodes.graphics.x(ksel)+dxy(1)
    GraphList.nodes.graphics.y(ksel)= GraphList.nodes.graphics.y(ksel)+dxy(2)
    ge_update_edges(karcs,modified)
    xy=rep(1:2)
    //draw modified objects at new position
    for k=1:nmoved,draw(moved(k)),end 
    for k=1:nmodified,draw(modified(k)),end 
     //draw(ge_axes_handle);
  end
  if rep(3)==2 then //cancel
    for k=1:nmoved,move(moved(k),[xc,yc]-rep(1:2)),end
    GraphList.nodes.graphics.x(ksel)=x_save
    GraphList.nodes.graphics.y(ksel)=y_save
  else
    ge_add_history(list("move",ksel,[x_save,y_save]))
  end
  ge_win_handle.immediate_drawing='on'
  ge_win_handle.pixel_drawing_mode='copy'
 
  ge_enablemenus()
  edited=return(edited)
endfunction

