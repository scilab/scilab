// Copyright INRIA

//
// sharks and sardins: Lotka-Volterra ODE
//

mode(-1)
xdel(winsid())
set("figure_style","old")
xbasc()
toolbar(0,'off');
titlepage(["Lotka-Volterra:";...
	"dy1/dt= 3*y1 - 2*y1*y2";..
	"dy2/dt=-2*y2 + y1*y2"]);
halt(); xbasc();


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

xbasc();
set("figure_style","old")
toolbar(0,'off');
xset("pixmap",1)
fchamp(f,1,fx,fy)
xset("font",2,12)
xtitle([""],["y1"],["y2"])
xset("wshow")

xset("alufunction",6)
xset("thickness",2)
t0=0; tmax=10;
t=t0:0.05:tmax;
oldx0=10*xmax; oldy0=10*ymax;
dx=0.1; dy=0.1;
rtol=0.0001; atol=rtol;
while (%t)
  [b,x0,y0]=xclick();
  if(b==5|b==2) then break end;
  if (b==3|b==0) & xmin<x0 & x0<xmax & ymin<y0 & y0<ymax then
    sol=ode([x0;y0],t0,t,rtol,atol,f);
    plot2d(sol(1,:),sol(2,:),5,"000")
    xset("wshow")
    rep=[x0,y0,1];
    while (rep(3)<>3) then
      rep=xgetmouse(0);
      x0=rep(1); y0=rep(2);
      if (xmin<x0 & x0<xmax & ymin<y0 & y0<ymax) &..
	  (abs(x0-oldx0)>=dx | abs(y0-oldy0)>=dy) then
        plot2d(sol(1,:),sol(2,:),5,"000")
        sol=ode([x0;y0],t0,t,rtol,atol,f);
        plot2d(sol(1,:),sol(2,:),5,"000")
	xset("wshow")
	oldx0=x0; oldy0=y0;
      end
    end
    xset("alufunction",1)
    plot2d(sol(1,:),sol(2,:),5,"000")
    xset("wshow")
    xset("alufunction",6)
  end
end
xset("pixmap",0)
xset("default")
lines(oldln(1))
