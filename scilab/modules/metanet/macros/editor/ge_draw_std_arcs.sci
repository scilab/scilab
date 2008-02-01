function ge_draw_std_arcs(xx,yy,Ids)
//Copyright INRIA
//Author : Serge Steer 2002

  if xx==[] then return,end
  xset("thickness",1)
  width=GraphList.edge_width(sel1);
  width(width==0)=GraphList.default_edge_width
  c=GraphList.edge_color(sel1)
  c(c==0)=1
  uwidth=unique(width)
  for w=uwidth
    k=find(width==w)
    xset("thickness",w)
    xpolys(xx(:,k),yy(:,k),c(k))
  end
  xx([1 4],:)=[];yy([1 4],:)=[];
  Ids=ge_get_arcs_id(sel1)
  
  if GraphList.directed|Ids<>[] then

    x0=(xx(2,:)+xx(1,:))/2;y0=(yy(2,:)+yy(1,:))/2;
    l=sqrt((xx(2,:)-xx(1,:)).^2+(yy(2,:)-yy(1,:)).^2);
    co=(xx(2,:)-xx(1,:))./l;si=-(yy(2,:)-yy(1,:))./l;
    x0=[x0+arrowWidth*si;x0+arrowLength*co;x0-arrowWidth*si;x0+arrowWidth*si]
    y0=[y0+arrowWidth*co;y0-arrowLength*si;y0-arrowWidth*co;y0+arrowWidth*co]
    if GraphList.directed then // draw the arrows
      for w=uwidth
	k=find(width==w)
	xset("thickness",w)
	xpolys(x0(:,k),y0(:,k),c(k))
      end
    end
    if Ids<>[] then//draw the arc identification
      f=GraphList.edge_font_size(sel1)
      f(f==0)=GraphList.default_font_size
      for k=1:size(Ids,'*')
	//rect=xstringl(x0(2,k),y0(1,k),Ids(k))
	xset("font size",ge_font(f(k)))
	xstring(x0(2,k),y0(1,k),Ids(k))
      end
    end
    end
    xset("thickness",1)
endfunction
