function ge_draw_loop_arcs(loops)
//Copyright INRIA
//Author : Serge Steer 2002
  if loops==[] then return,end
  vtail=GraphList.tail(loops)
  width=GraphList.edge_width(loops);
  width(width==0)=GraphList.default_edge_width
  uwidth=unique(width)
  c=GraphList.edge_color(loops)
  c(c==0)=1
  [u,ii]=unique(vtail);

  Ids=ge_get_arcs_id(loops)
  arcs=[];cols=[];X=[],Y=[]

  for iii=ii //loop on nodes
    x1=GraphList.node_x(vtail(iii));y1=GraphList.node_y(vtail(iii)); 
    uni=GraphList.node_diam(vtail((iii)));
    uni(uni==0)=GraphList.default_node_diam;
    jj=find(vtail==vtail(iii));cols=[cols,c(jj)];
    h=((1:size(jj,'*'))/2+0.5)*uni;

    DX=1.2*h;DY=h;
    //computation of the intersections of the ellipses (arcs) with a
    //circle (nodes)
    //suppose the equations are
    // x=uni*sin(theta)+x0; y=uni*cos(theta)+y0  // the circle
    // x=DX/2*sin(alpha)+x0; y=DY/2*cos(alpha)+y0+DY/2 // the ellipse
    costheta= ((-2*DX^2+2*sqrt(DX^4-uni^2*DX^2+uni^2*DY^2)).*DY/2)./(-uni*DX^2+uni*DY^2)
    da=180-180*acos((uni*costheta-DY)./DY)/%pi
    arcs(1:6,jj)=[x1-DX/2; y1+DY;DX;DY;64*(-90+da);64*(360-2*da)];
    X(1,jj)=x1*ones(jj)
    Y(1,jj)=(y1+DY)
  end 
  for w=uwidth
    k=find(width==w)
    xset("thickness",w)
    xarcs(arcs(:,k),cols(k));
  end
  if GraphList.directed then // draw the arrows
    x0=[X;X+arrowLength;X;X]
    y0=[Y+arrowWidth;Y;Y-arrowWidth;Y+arrowWidth]
    for w=uwidth
      k=find(width==w)
      xset("thickness",w)
      xpolys(x0(:,k),y0(:,k),cols(k))
    end
  end
  if Ids<>[] then//draw the arc identification
    f=GraphList.edge_font_size(loops)
    f(f==0)=GraphList.default_font_size
 
    for k=1:size(Ids,'*')
      xset("font size",ge_font(f(k)))
      rect=xstringl(0,0,Ids(k)) 
      xstring(X(k)-rect(3),Y(k),Ids(k))
    end
  end
endfunction
