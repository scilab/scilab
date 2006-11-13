// Copyright INRIA

//
// sharks and sardins: Lotka-Volterra ODE
//

mode(-1)
xdel(winsid())
toolbar(0,'off');
titlepage(["Lotka-Volterra:";...
	"dy1/dt= 3*y1 - 2*y1*y2";..
	"dy2/dt=-2*y2 + y1*y2"]);
halt(); clf();


mode(1)
oldln=lines();
lines(0)

deff("yprim=f(t,y)",..
    ["yprim1=aa*y(1)-cc*y(1)*y(2)";..
     "yprim2=-bb*y(2)+ff*y(1)*y(2)";..
     "yprim=[yprim1;yprim2]"])

aa=3; bb=2; cc=2; ff=1;

xmin=0; xmax=4; ymin=0; ymax=4;
fx=xmin:0.5:xmax; fy=ymin:0.5:ymax;

toolbar(0,'off');
fchamp(f,1,fx,fy)
a=gca();
a.x_label.text="y1",a.x_label.font_size=3;
a.y_label.text="y2",a.y_label.font_size=3;
a.title.text='Click left to start a new trajectory, move the mouse@and click again to fix the desired trajectory@Click right to exit';

t0=0; tmax=10;
t=t0:0.05:tmax;
oldx0=10*xmax; oldy0=10*ymax;
dx=0.1; dy=0.1;
rtol=0.0001; atol=rtol;
fig=gcf();fig.pixmap='on';
show_pixmap();

while (%t)
  [b,x0,y0]=xclick();
  if or(b==[2 5]) then break end;
  if or(b==[0 3]) & xmin<x0 & x0<xmax & ymin<y0 & y0<ymax then
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
