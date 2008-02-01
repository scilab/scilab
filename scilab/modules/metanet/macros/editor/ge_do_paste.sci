function GraphList=ge_do_paste(GraphList,xc,yc)
//Copyright INRIA
//Author : Serge Steer 2002

  global GClipBoard
  if GClipBoard==[] then return,end
  ge_disablemenus()
  reg=GClipBoard(1)
  if size(reg)==0 then return,end
  rect=matrix(GClipBoard(2),-1,1);
  xo=rect(1);yo=rect(2),rect(1)=xc;rect(2)=yc;
  [rect,btn]=dragrect(rect)

  if btn==2 then ge_enablemenus();edited=%f,return,end
  
  reg.node_x=reg.node_x-xo+rect(1)
  reg.node_y=reg.node_y-yo+rect(2)-rect(4)

  nn=size(GraphList.node_x,'*')
  for f=ge_node_fields(),
     GraphList(f)=[GraphList(f),reg(f)];
  end
  ge_drawnodes(nn+1:$)
  if reg.head<>[] then 
    reg.head=reg.head+nn;
    reg.tail=reg.tail+nn;
  end
  na=size(GraphList.head,'*')
  for f=ge_arc_fields()
    GraphList(f)=[GraphList(f),reg(f)];
  end
  ge_drawarcs(na+1:$)
  ge_add_history(list("paste",nn,na))
  ge_enablemenus();
  edited=return(%t)
endfunction
