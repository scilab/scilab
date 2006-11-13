// Copyright INRIA

//
// Van der Pol ODE
//

mode(-1)
xdel(winsid())
toolbar(0,'off');
titlepage(["Van der Pol:";...
	   "dy1/dt= y2";..
	   "dy2/dt=5*(1-y1^2)*y2-y1"]);
halt(); 

mode(1)
oldln=lines();
lines(0)

deff("yprim=f(t,y)",..
     ["yprim1=y(2)";..
      "yprim2=mu*(1-y(1)^2)*y(2)-y(1)";..
      "yprim=[yprim1;yprim2]";])

mu=5;

xmin=-3; xmax=3; ymin=-8; ymax=8;
xr=xmin:0.5:xmax; yr=ymin:1:ymax;

clf();
toolbar(0,'off');
fchamp(f,1,xr,yr)
a=gca();
a.x_label.text="y1",a.x_label.font_size=3;
a.y_label.text="y2",a.y_label.font_size=3;
a.title.text='Click left to start a new trajectory, move the mouse@and click again to fix the desired trajectory@Click right to exit';

fig=gcf();fig.pixmap='on';
show_pixmap();
t0=0; dt=0.05; tmax=15;
t=t0:dt:tmax;
oldx0=10*xmax; oldy0=10*ymax;
dx=0.1; dy=0.1;
rtol=0.0001; atol=rtol;
while (%t)
  [b,x0,y0]=xclick();
  if or(b==[2 5]) then break,end;
  if or(b==[0 3]) & x0>=xmin & x0<=xmax & y0>=ymin & y0<=ymax then
    sol=ode([x0;y0],t0,t,rtol,atol,f);
    xpoly(sol(1,:)',sol(2,:)');
    p=gce();p.thickness=2;p.foreground=5;
    show_pixmap()
    rep=[x0,y0,-1];  
    while rep(3)==-1 then
      rep=xgetmouse(0);
      x0=rep(1); y0=rep(2);
      if (xmin<x0 & x0<xmax & ymin<y0 & y0<ymax) &..
	    (abs(x0-oldx0)>=dx | abs(y0-oldy0)>=dy) then
	sol=ode([x0;y0],t0,t,rtol,atol,f);
	p.data=[sol(1,:)' sol(2,:)'];
	show_pixmap()
	oldx0=x0; oldy0=y0;
      end
    end
  end
end
fig.pixmap='off';
lines(oldln(1))
