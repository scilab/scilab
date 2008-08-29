
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function  GraphList=ge_do_move_region(GraphList,xc,yc)
  ge_disablemenus()
  reg=list();rect=[]
  drawnow()

  [rect,btn]=rubberbox([xc,yc])
  if btn==2 then ge_enablemenus();return,end

  
  [ksel,del]=ge_get_nodes_in_rect(GraphList,rect);
  edited=ksel<>[]
  if ~edited then ge_enablemenus();return,end

  //find arcs connected with at least one internal node
  karcs=find(dsearch(GraphList.edges.tail,ksel,'d')>0|...
	     dsearch(GraphList.edges.head,ksel,'d')>0)
  
  //find arcs connected with two internal node
  kinternals=find(dsearch(GraphList.edges.tail,ksel,'d')>0& ...
		  dsearch(GraphList.edges.head,ksel,'d')>0)
  //find arcs connected with only one internal node
  if kinternals<>[] then
    krm=find(dsearch(karcs,kinternals,'d')>0)
    karcs(krm)=[]
  end
  
  x_save=GraphList.nodes.graphics.x(ksel);
  y_save=GraphList.nodes.graphics.y(ksel);

  ge_win_handle=gcf()
  ge_win_handle.immediate_drawing='off'
  ge_win_handle.pixel_drawing_mode='equiv'
  ge_axes_handle=gca();n=size(ge_axes_handle.children,'*')
  gindex=ge_axes_handle.user_data;
  hedges=gindex.edge;hnodes=gindex.node;
  

  moved=[hnodes(ksel);hedges(kinternals)]
  nmoved=size(moved,'*')
  modified=hedges(karcs)
  nmodified=size(modified,'*')
  
  rep(3)=-1
  xy=[rect(1)+rect(3),rect(2)-rect(4)]
  xy_save=xy;
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
    //show_pixmap()
  end
  if rep(3)==2 then //cancel
    for k=1:nmoved,move(moved(k),xy_save-rep(1:2)),end
    GraphList.nodes.graphics.x(ksel)=x_save
    GraphList.nodes.graphics.y(ksel)=y_save
    ge_update_edges(karcs,modified)
  else
    ge_add_history(list("move_region",ksel,xy-xy_save))
    ge_enablemenus()
  end
  ge_win_handle.immediate_drawing='on'
  ge_win_handle.pixel_drawing_mode='copy'
  show_pixmap()
  ge_enablemenus()
  edited=return(%t)

endfunction
