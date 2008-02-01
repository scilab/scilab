function GraphList=ge_do_paste(GraphList,xc,yc)
//Copyright INRIA
//Author : Serge Steer 2002
  ge_axes_handle=gca(); 
  gindex=ge_axes_handle.user_data;
  hedges=gindex.edge;hnodes=gindex.node;


  global GClipBoard
  if GClipBoard==[] then return,end
  ge_disablemenus()
  reg=GClipBoard(1)
  if size(reg)==0 then return,end
  rect=matrix(GClipBoard(2),-1,1);
  xo=rect(1);yo=rect(2),rect(1)=xc;rect(2)=yc;
  [rect,btn]=dragrect(rect)
  drawlater()

  if btn==2 then ge_enablemenus();edited=%f,return,end
  
  reg.nodes.graphics.x=reg.nodes.graphics.x-xo+rect(1)
  reg.nodes.graphics.y=reg.nodes.graphics.y-yo+rect(2)-rect(4)

  nn=size(GraphList.nodes)
  nn1=size(reg.nodes)
  GraphList.nodes(nn+(1:nn1))=reg.nodes
  for k=nn+1:nn+nn1,  hnodes(k)=ge_draw_node(k); end

  na=size(GraphList.edges)
  na1=size(reg.edges)
  if na1>0 then 
    reg.edges.head=reg.edges.head+nn;
    reg.edges.tail=reg.edges.tail+nn;
    GraphList.edges(na+(1:na1))=reg.edges;
    for k=na+1:na+na1, hedges(k)=ge_draw_edge(k), end
  end
  gindex.edge=hedges;gindex.node=hnodes;
  ge_axes_handle.user_data=gindex

  drawnow()
  show_pixmap()
  ge_add_history(list("paste",nn,na))
  ge_enablemenus();
  edited=return(%t)
endfunction
