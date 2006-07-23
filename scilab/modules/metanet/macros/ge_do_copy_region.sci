function GraphList=ge_do_copy_region(GraphList,xc,yc)
//Copyright INRIA
//Author : Serge Steer 2002

  ge_disablemenus()
  curwin=xget('window')
  [reg,rect]=ge_get_region(xc,yc,curwin)
  // Copyright INRIA
  if rect==[] then ge_enablemenus();return,end
  modified=length(reg)>1


  xinfo('Drag to destination position and click (left to fix, right to cancel)')
  rep(3)=-1
  yc=yc-rect(4)  
  dr=driver()
  if dr=='Rec' then driver('X11'),end
  pat=xget('pattern')
  xset('pattern',0)
  //move loop
  while rep(3)==-1 then 
    // draw region shape
    xrect(xc,yc+rect(4),rect(3),rect(4))
    if xget('pixmap') then xset('wshow'),end
    // get new position
    rep=xgetmouse()
    // erase block shape
    xrect(xc,yc+rect(4),rect(3),rect(4))
    if xget('pixmap') then xset('wshow'),end
    xc=rep(1);yc=rep(2)
    xy=[xc,yc];
  end

  driver(dr);xset('pattern',pat)
  if rep(3)==2 then ge_enablemenus();driver(dr),return,end

  nold=size(GraphList.node_x,'*')
  reg.node_x=reg.node_x-rect(1)+xc
  reg.node_y=reg.node_y-rect(2)+yc
  ge_drawobjs(reg)
  for k=1:size(reg.node_x,'*')
    for f=ge_node_fields(),GraphList(f)($+1)=reg(f)(k),end
  end
  reg.head=reg.head+nold
  reg.tail=reg.tail+nold
  for k=1:size(reg.head,'*')
    for f=ge_arc_fields(),GraphList(f)($+1)=reg(f)(k),end
  end
  ge_enablemenus()
  edited=resume(%t)

endfunction
