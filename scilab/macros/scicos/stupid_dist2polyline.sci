function [d,pt,ind] = stupid_dist2polyline(xp,yp,pt,pereps)
// Copyright INRIA
// computes minimum distance from a point to a polyline
// d    minimum distance to polyline
// pt   coordinate of the polyline closest point
// ind  
//      if negative polyline closest point is a polyline corner:
//         pt=[xp(-ind) yp(-ind)]
//      if positive pt lies on segment [ind ind+1]
//
  x = pt(1) ;
  y = pt(2) ;
  xp = xp(:); yp=yp(:)
  cr = 4*sign((xp(1:$-1)-x).*(xp(1:$-1)-xp(2:$))+..
	      (yp(1:$-1)-y).*(yp(1:$-1)-yp(2:$)))+..
         sign((xp(2:$)-x).*(xp(2:$)-xp(1:$-1))+..
              (yp(2:$)-y).*(yp(2:$)-yp(1:$-1)))

  ki = find(cr==5) // index of segments for which projection fall inside
  np = size(xp,'*')
  if ki<>[] then
    //projection on segments
    x = [xp(ki) xp(ki+1)];
    y = [yp(ki) yp(ki+1)];
    dx = x(:,2)-x(:,1);
    dy = y(:,2)-y(:,1);
    d_d = dx.^2 + dy.^2 ;
    d_x = ( dy.*(-x(:,2).*y(:,1)+x(:,1).*y(:,2))+dx.*(dx*pt(1)+dy*pt(2)))./d_d
    d_y = (-dx.*(-x(:,2).*y(:,1)+x(:,1).*y(:,2))+dy.*(dx*pt(1)+dy*pt(2)))./d_d
    xp  = [xp;d_x]
    yp  = [yp;d_y]
  end

  zzz = [ones(np,1) ; zeros(size(ki,'*'),1)] * eps
  zz  = [ones(np,1)*pereps ; ones(size(ki,'*'),1)]
  [d,k] = min(sqrt((xp-pt(1)).^2+(yp-pt(2)).^2).*zz - zzz) 
  pt(1) = xp(k)
  pt(2) = yp(k)
  if k>np then ind=ki(k-np), else ind=-k, end
  
endfunction

