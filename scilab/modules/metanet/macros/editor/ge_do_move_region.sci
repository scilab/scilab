function  GraphList=ge_do_move_region(GraphList,xc,yc)
//Copyright INRIA
//Author : Serge Steer 2002

  ge_disablemenus()
  reg=list();rect=[]
  [rect,btn]=rubberbox([xc,yc])
  if btn==2 then ge_enablemenus();return,end

  
  [keep,del]=ge_get_nodes_in_rect(GraphList,rect);
  edited=keep<>[]
  if ~edited then ge_enablemenus();return,end

  karcs=[]
  for ksel=keep
    //get arcs connected to this node
    karcs=[karcs,find(GraphList.tail==ksel|GraphList.head==ksel)];
  end
  karcs=unique(karcs)

  [nrect,btn]=dragrect(rect)
  ge_drawarcs(karcs);ge_drawnodes(keep) //erase
  GraphList.node_x(keep)=GraphList.node_x(keep)-rect(1)+nrect(1);
  GraphList.node_y(keep)=GraphList.node_y(keep)-rect(2)+nrect(2);
  ge_drawnodes(keep);ge_drawarcs(karcs) //redraw at the new position
  ge_add_history(list("move_region",keep,[rect(1)-nrect(1),rect(2)-nrect(2)]))
  ge_enablemenus()
  edited=return(%t)
endfunction
