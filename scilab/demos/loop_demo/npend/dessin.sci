function []=chaind(p,r,rect)
//draw chain given sequence of points
//!
//
// Copyright ENPC
n=maxi(size(p));
arcs=[ -r ,r,2*r,2*r,0,64*360].*.ones(n,1);
arcs=[ p , 0*ones(n,4)] + arcs;
xarcs(arcs');
th=xget("thickness");
xset("thickness",4);
xpoly(p(:,1),p(:,2),"lines");
xset("thickness",th);


function []=chaina(yt)
[n1,n2]=size(yt);
x=ones(n1/2,n2);
y=ones(n1/2,n2);
x(1,:)=2*r(1)*cos(yt(1,:));
y(1,:)=2*r(1)*sin(yt(1,:));
for i=2:n1/2, 
    x(i,:)=x(i-1,:)+2*r(i)*cos(yt(i,:));
    y(i,:)=y(i-1,:)+2*r(i)*sin(yt(i,:));
end
x=[0*ones(1,n2);x];y=[0*ones(1,n2);y];
rr=r(1)/(n1+1);
rect=2*[-(n1/2+1),-(n1/2+1),n1/2+1,n1/2+1];
if driver()<>'Pos' then
  plot2d(0,0,[0],"012"," ",rect);
  xset("alufunction",6);
  for j=1:n2,chaind([x(:,j),y(:,j)],rr,rect);
    chaind([x(:,j),y(:,j)],rr,rect);
  end
  xset("alufunction",3);
else
  pix=xget('pixmap');xset('pixmap',1)
  for j=1:3:n2,
    xset("wwpc");
    xpoly(rect([1 3 3 1]),rect([2,2,4,4]),'lines',1)
    chaind([x(:,j),y(:,j)],rr,rect);
    xset('wshow')
  end
  xset('pixmap',pix)
end

function []=chainb(yt)
[n1,n2]=size(yt);
x=ones(n1/2,n2);y=ones(n1/2,n2);
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
xx=x(n1/2+1,:)
yy=y(n1/2+1,:)
//plot2d(xx',yy',1,"000");
if driver()<>'Pos' then
  xset("alufunction",6);
  for j=1:n2,
    if j>=2,xsegs([xx(1,j-1);xx(1,j)],[yy(1,j-1);yy(1,j)]);end
    chaind([x(:,j),y(:,j)],rr,rect);
    chaind([x(:,j),y(:,j)],rr,rect);
  end
  xset("alufunction",3);
else
  pix=xget('pixmap');xset('pixmap',1)
  for j=1:3:n2,
    xset("wwpc");
    xpoly(rect([1 3 3 1]),rect([2,2,4,4]),'lines',1)
    if j>=2,xsegs([xx(1,j-1);xx(1,j)],[yy(1,j-1);yy(1,j)]);end
    chaind([x(:,j),y(:,j)],rr,rect);
    xset('wshow')
  end
  xset('pixmap',pix)
end




