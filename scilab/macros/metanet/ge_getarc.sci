function k=ge_getarc(GraphList,pt)
//Copyright INRIA
//Author : Serge Steer 2002

// Copyright INRIA
  [xx,yy,sel1,loops]=ge_arc_coordinates(:)
  k=dist2polylines(xx,yy,pt)
  if k<>[] then
    k=sel1(k)
    return
  end

  k=[]
  if loops==[] then return,end
  
    
  vtail=GraphList.tail(loops)
  [u,ii]=unique(vtail);

  for iii=ii //loop on nodes
    x1=GraphList.node_x(vtail(iii));y1=GraphList.node_y(vtail(iii)); 
    uni=GraphList.node_diam(vtail((iii)));

    jj=find(vtail==vtail(iii));
    h=((1:size(jj,'*'))/2+0.5)*uni;

    RX=1.2*h/2;RY=h/2;

    d=abs(((pt(1)-x1)./RX)^2+((pt(2)-y1-RY)./RY)^2-1)
    [m,k]=mini(d)
    if m<0.3 then
      k=loops(jj(k));break
    else
      k==[]
    end
  end 

endfunction
