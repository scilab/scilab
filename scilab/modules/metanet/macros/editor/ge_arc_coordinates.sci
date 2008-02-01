function [xx,yy,sel,loops]=ge_arc_coordinates(sel)
//Copyright INRIA
//Author : Serge Steer 2002
  ArcDx=0.25;ArcDy=15;
  xx=[];yy=[];loops=[]
  
  if size(sel,1)==-1 then sel=1:size(GraphList.tail,'*'),end
  vtail=GraphList.tail(sel);
  vhead=GraphList.head(sel);
  if vtail==[] then return,end
  
  //remove loops arcs
  loops=find(vtail==vhead) //arcs which are  loops
  vhead(loops)=[]
  vtail(loops)=[]
  sel(loops)=[]
  
  if vhead==[] then return,end
  //remove hidden arcs
  l = sqrt((GraphList.node_x(vhead)-GraphList.node_x(vtail)).^2+..
	   (GraphList.node_y(vhead)-GraphList.node_y(vtail)).^2)
  dt=GraphList.node_diam(vtail);dt(dt==0)=GraphList.default_node_diam
  dh=GraphList.node_diam(vhead);dh(dh==0)=GraphList.default_node_diam
  
  hidden=find(l<=dt/2+dh/2)
  l(hidden)=[]
  vhead(hidden)=[]
  vtail(hidden)=[]
  dt(hidden)=[]
  dh(hidden)=[]
  sel(hidden)=[]
  if vtail==[] then return,end
  
  //compute number of arcs between each couple of nodes
  n=max([vtail vhead ]);
  spp=spzeros(n,n);
  for i=1:size(vtail,'*')
    ii=vtail(i); kk=vhead(i);
    spp(ii,kk)=spp(ii,kk)+1; at=spp(ii,kk)+spp(kk,ii);v(1,i)=at-1;
  end
  epai=((-1).^v).*round(v/2).*sign(vhead-vtail);
  clear spp v
    
  
  //cosine and sine of the line which joins head to tail
  co=(GraphList.node_x(vhead)-GraphList.node_x(vtail))./l
  si=-(GraphList.node_y(vhead)-GraphList.node_y(vtail))./l
  
  l=ArcDx * (l-dt/2-dh/2);
  
  x0=GraphList.node_x(vtail)+(dt/2) .*co
  y0=GraphList.node_y(vtail)-(dt/2) .*si
  x1=x0+l.*co - ArcDy* epai.*si
  y1=y0-(l.*si + ArcDy* epai.*co)
  x2=x1 + (1/ArcDx-2)*l.*co
  y2=y1 - (1/ArcDx-2)*l.*si
  x3=GraphList.node_x(vhead)-(dh/2) .*co
  y3=GraphList.node_y(vhead)+(dh/2) .*si
  clear co si epai l vhead vtail dt dh
    
  xx=[x0;x1;x2;x3];clear x0 x1 x2 x3
  yy=[y0;y1;y2;y3];clear y0 y1 y2 y3
  
endfunction
