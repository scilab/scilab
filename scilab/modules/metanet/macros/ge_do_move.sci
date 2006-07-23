function GraphList=ge_do_move(GraphList,xc,yc)
//Copyright INRIA
//Author : Serge Steer 2002

// Copyright INRIA
// get a scicos object to move, and move it with connected objects
//!
  
//get block to move

  ksel=ge_getnode(GraphList,[xc;yc])
  edited=ksel<>[]
  if ~edited then return,end
  
  //get arcs connected to this node
  karcs=find(GraphList.tail==ksel|GraphList.head==ksel)
  x_save=GraphList.node_x(ksel);
  y_save=GraphList.node_y(ksel);
  rep(3)=-1
  ge_drawobjs(GraphList,ksel,karcs) //erase
  dr=driver();if dr=='Rec' then driver('X11'),end
  ge_drawobjs(GraphList,ksel,karcs) //redraw
  while rep(3)==-1 ,  // move loop
    rep=xgetmouse()
    ge_drawobjs(GraphList,ksel,karcs) //erase
    GraphList.node_x(ksel)=rep(1)
    GraphList.node_y(ksel)=rep(2)
    ge_drawobjs(GraphList,ksel,karcs) //draw at the position
  end
  if rep(3)==2 then
    ge_drawobjs(GraphList,ksel,karcs) //erase
    GraphList.node_x(ksel)=x_save
    GraphList.node_y(ksel)=y_save
    driver(dr)
    ge_drawobjs(GraphList,ksel,karcs) 
    return
  else
    ge_drawobjs(GraphList,ksel,karcs) //erase
    driver(dr);ge_drawobjs(GraphList,ksel,karcs) //draw and record
    ge_add_history(list("move",ksel,[x_save,y_save]))
    edited=return(edited)
  end
endfunction

