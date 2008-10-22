//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

function chainap(yt)
  [n1,n2]=size(yt);
  x=ones(n1/2,n2);
  y=ones(n1/2,n2);
  x(1,:)=2*r(1)*cos(yt(1,:));
  y(1,:)=2*r(1)*sin(yt(1,:));
  for i=2:n1/2, 
    x(i,:)=x(i-1,:)+2*r(i)*cos(yt(i,:));
    y(i,:)=y(i-1,:)+2*r(i)*sin(yt(i,:));
  end
  x=[0*ones(1,n2);x];
  y=[0*ones(1,n2);y];
  rr=r(1)/(n1+1);
  rect=2*[-(n1/2+1),-(n1/2+1),n1/2+1,n1/2+1];
  plot2d(0,0,[0],"012"," ",rect);
  show_pixmap();
  if driver()=='Pos' then st=3;else st=1;end
  J=1:st:n2;
  p = [x(:,J(1)),y(:,J(1))];

  [n,ign]=size(p);

  arcs_base=[ -rr ,rr,2*rr,2*rr,0,64*360].*.ones(n,1);
  arcs=[ p, 0*ones(n,4)] + arcs_base;
  xarcs(arcs');
  e1 = gce(); // coupound with 11 arcs as children
  e1.children(:).thickness = 1;

  colors=1:n;colors(8)=n+1;
  xpolys([p(1:(n-1),1),p(2:n,1)]',[p(1:(n-1),2),p(2:n,2)]',colors);
  e2 = gce(); //compound with 10 polylines as children
  e2.children(:).thickness = 4;  
  show_pixmap();
  realtimeinit(0.04)
  for j=1:max(size(J))
    realtime(j)
    drawlater();
    p = [x(:,J(j)),y(:,J(j))];
    arcs = [ p, 0*ones(n,4)] + arcs_base;
    for i = 1:n-1      
      e1.children(i).data = arcs(i,:);
      e2.children(i).data(:,1) = [x(n-i,J(j)),x(n+1-i,J(j))]';
      e2.children(i).data(:,2) = [y(n-i,J(j)),y(n+1-i,J(j))]';
    end
    drawnow();
    show_pixmap();
  end
endfunction

