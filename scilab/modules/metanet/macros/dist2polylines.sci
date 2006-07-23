function [k]=dist2polylines(xp,yp,pt)
//Copyright INRIA
//Author : Serge Steer 2002

// computes minimum distance from a point to a set of polylines
//xp xp : polylines coordinates (one polyline by column)
//d    minimum distance to polyline
//pt   coordinate of the polyline closest point
//ind  
//     if negative polyline closest point is a polyline corner:pt=[xp(-ind) yp(-ind)]
//     if positive pt lies on segment [ind ind+1]

// Copyright INRIA
//mxx=max(xp)/10;mxy=max(yp)/10;
//mxx=0;mxy=0;
//ki=find((xp(1:$-1,:)<=(pt(1)+mxx)&(pt(1)-mxx)<xp(2:$,:))&..
//	(yp(1:$-1,:)<=(pt(2)+mxy)&(pt(2)-mxy)<yp(2:$,:)))

  x=pt(1);y=pt(2);

  cr=4*sign((xp(1:$-1,:)-x).*(xp(1:$-1,:)-xp(2:$,:))+..
	    (yp(1:$-1,:)-y).*(yp(1:$-1,:)-yp(2:$,:)))+..
     sign((xp(2:$,:)-x).*(xp(2:$,:)-xp(1:$-1,:))+..
          (yp(2:$,:)-y).*(yp(2:$,:)-yp(1:$-1,:)))
  cr($+1,:)=0
  ki=find(cr==5);

  if ki<>[] then
    //projection on segments
    x=[xp(ki) xp(ki+1)];
    y=[yp(ki) yp(ki+1)];
    dx=x(:,2)-x(:,1);
    dy=y(:,2)-y(:,1);
    d_d=dx.^2+dy.^2;

    kz=find(d_d==0);d_d(kz)=1

    d_x=( dy.*(-x(:,2).*y(:,1)+x(:,1).*y(:,2))+dx.*(dx*pt(1)+dy*pt(2)))./d_d;
    d_y=(-dx.*(-x(:,2).*y(:,1)+x(:,1).*y(:,2))+dy.*(dx*pt(1)+dy*pt(2)))./ d_d;
    D=(d_x-pt(1)).^2+(d_y-pt(2)).^2 //distance with all points
    D(kz)=%inf
    [d,k]=min(D) //distance with all points
    if d<2 then
      k=floor((ki(k)-1)/size(xp,1))+1
    else
      k=[]
    end
  else 
    k=[]
  end
endfunction
