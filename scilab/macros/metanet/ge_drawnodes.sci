function ge_drawnodes(sel)
//Copyright INRIA
//Author : Serge Steer 2002

  thick=xget("thickness")
  if or(type(sel)==[2 129]) then 
    sel=horner(sel,size(GraphList.node_x,'*')),
  elseif size(sel,1)==-1 then 
    sel=1:size(GraphList.node_x,'*'),
  end
  t=GraphList.node_type(sel)
  tu=unique(t)
  for tu=unique(t)
    execstr('ge_drawnodes_'+string(tu)+'(sel(t==tu))')
  end
  if xget('pixmap') then xset('wshow'),end
  xset("thickness",thick)
endfunction

function ge_drawnodes_0(sel)
  d=matrix(GraphList.node_diam(sel),1,-1)
  if d==[] then return,end
  d(d==0)=GraphList.default_node_diam;
  x=matrix(GraphList.node_x(sel),1,-1)
  y=matrix(GraphList.node_y(sel),1,-1)
  b=matrix(GraphList.node_border(sel),1,-1)
  b(b==0)=GraphList.default_node_border
  c=matrix(GraphList.node_color(sel),1,-1)
  arcs=[x-d/2
	y+d/2
	d
	d
	0*d
	360*64*ones(d)];
  for bu=unique(b)
    k=find(b==bu)
    xset("thickness",bu)
    xarcs(arcs(:,k),c(k));
  end
  
  Ids=ge_get_nodes_id(sel)
  if Ids<>[] then//ge_draw the node identification
    f=GraphList.node_font_size(sel)
    f(f==0)=GraphList.default_font_size

    for k=1:size(d,'*')
      xset("font size",ge_font(f(k)))
      r=xstringl(0,0,Ids(k))
      xstring(x(k)-r(3)/2,y(k)-d(k)/2-r(4)*1.5,Ids(k))
    end
  end
endfunction

function ge_drawnodes_2(sel)
  //PLOT OF SOURCE NODES
  d=matrix(GraphList.node_diam(sel),1,-1)
  if d==[] then return,end
   d(d==0)=GraphList.default_node_diam;
  b=matrix(GraphList.node_border(sel),1,-1)
  b(b==0)=GraphList.default_node_border
  c=matrix(GraphList.node_color(sel),1,-1)
  x=matrix(GraphList.node_x(sel),1,-1)
  y=matrix(GraphList.node_y(sel),1,-1)
  arcs=[x-d/2
	y+d/2
	d
	d
	0*d
	360*64*ones(d)];
  vx=[-1/2;-1/2;-3/4;-1;0;+1;+3/4;+1/2;+1/2;-1/2]
  vy=[1/2;3/2;1;1;5/2;1;1;3/2;1/2;1/2];
  for bu=unique(b)
    k=find(b==bu)
    xset("thickness",bu)
    xarcs(arcs(:,k),c(k));
    xpolys(ones(10,1)*x+vx*d(k),ones(10,1)*y+vy*d(k),max(c(k),1))
  end
  
  Ids=ge_get_nodes_id(sel)
  if Ids<>[] then//draw the node identification
    f=GraphList.node_font_size(sel)
    f(f==0)=GraphList.default_font_size

    for k=1:size(d,'*')
      xset("font size",ge_font(f(k)))
      r=xstringl(0,0,Ids(k))
      xstring(x(k)-r(3)/2,y(k)-d(k)/2-r(4)*1.5,Ids(k))
    end
  end

endfunction

function ge_drawnodes_1(sel)
  //	PLOT OF SINK NODES
  d=matrix(GraphList.node_diam(sel),1,-1)
  if d==[] then return,end
  d(d==0)=GraphList.default_node_diam;
  b=matrix(GraphList.node_border(sel),1,-1)
  b(b==0)=GraphList.default_node_border
  c=matrix(GraphList.node_color(sel),1,-1)
  x=matrix(GraphList.node_x(sel),1,-1)
  y=matrix(GraphList.node_y(sel),1,-1)
  arcs=[x-d/2
	y+d/2
	d
	d
	0*d
	360*64*ones(d)];
  vx=[-1/2;-1/2;-3/4;-1;0;+1;+3/4;+1/2;+1/2;-1/2]
  vy=[1/2;3/2;1;1;5/2;1;1;3/2;1/2;1/2]-3;
  for bu=unique(b)
    k=find(b==bu)
    xset("thickness",bu)
    xarcs(arcs(:,k),c(k));
    xpolys(ones(10,1)*x+vx*d(k),ones(10,1)*y+vy*d(k),max(c(k),1))
  end
  
  Ids=ge_get_nodes_id(sel)
  if Ids<>[] then//draw the node identification
    f=GraphList.node_font_size(sel)
    f(f==0)=GraphList.default_font_size
    
    for k=1:size(d,'*')
      xset("font size",ge_font(f(k)))
      r=xstringl(0,0,Ids(k))
      xstring(x(k)-r(3)/2,y(k)+d(k)/2+r(4)*0.5,Ids(k))
    end
  end
endfunction

function ge_drawnodes_3(sel,id)
  //	PLOT OF Square NODES
  d=matrix(GraphList.node_diam(sel),1,-1)/2
  if d==[] then return,end
  d(d==0)=GraphList.default_node_diam;  
  b=matrix(GraphList.node_border(sel),1,-1)
  b(b==0)=GraphList.default_node_border
  c=matrix(GraphList.node_color(sel),1,-1)
  x=matrix(GraphList.node_x(sel),1,-1)
  y=matrix(GraphList.node_y(sel),1,-1)
  r=[x-d/2;
     y+d/2;
     d;
     d];
  for bu=unique(b)
    k=find(b==bu)
    xset("thickness",bu)
    xrects(r(:,k),-c(k));
  end
  
  Ids=ge_get_nodes_id(sel)
  if Ids<>[] then//draw the node identification
    f=GraphList.node_font_size(sel)
    f(f==0)=GraphList.default_font_size
    for k=1:size(d,'*')
      xset("font size",ge_font(f(k)))
      r=xstringl(0,0,Ids(k))
      xstring(x(k)-r(3)/2,y(k)-d(k)/2-r(4)*1.5,Ids(k))
    end
  end

endfunction

function Ids=ge_get_nodes_id(sel)
  execstr('NodeId=EGdata_'+string(win)+'.NodeId')
  Ids=[]
  select NodeId
    case 1 then Ids=string(sel)
    case 2 then Ids=GraphList.node_name(sel)
    case 3 then Ids=string(GraphList.node_demand(sel))
    case 4 then Ids=string(GraphList.node_label(sel))
  end
endfunction
