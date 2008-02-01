function  GraphList=ge_do_move_region(GraphList,xc,yc)
//Copyright INRIA
//Author : Serge Steer 2002
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
  ge_axes_handle=gca();n=size(ge_axes_handle.children,'*')
  gindex=ge_axes_handle.user_data;
  hedges=gindex.edge;hnodes=gindex.node;
  

  moved=[hnodes(ksel);hedges(kinternals)]
  nmoved=size(moved,'*')
  modified=hedges(karcs)
  
  rep(3)=-1
  xy=[rect(1)+rect(3),rect(2)-rect(4)]
  xy_save=xy;
  while rep(3)==-1 ,  // move loop
    rep=xgetmouse()
    dxy=rep(1:2)-xy
    for k=1:nmoved,move(moved(k),dxy),end
    GraphList.nodes.graphics.x(ksel)=GraphList.nodes.graphics.x(ksel)+dxy(1)
    GraphList.nodes.graphics.y(ksel)= GraphList.nodes.graphics.y(ksel)+dxy(2)
    ge_update_edges(karcs,modified)
    xy=rep(1:2)
    draw(ge_axes_handle);
    show_pixmap()
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
  show_pixmap()
  ge_enablemenus()
  edited=return(%t)

endfunction
