// Copyright INRIA

//
// Van der Pol ODE
//

mode(-1)
xbasc()
titlepage(["Van der Pol:";...
	"dy1/dt= y2";..
	"dy2/dt=5*(1-y1^2)*y2-y1"]);
halt(); xbasc()

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

xbasc();set("figure_style","old")
xset("pixmap",1)
fchamp(f,1,xr,yr)
xset("font",2,12) 
xtitle([""],["y1"],["y2"])
xset("wshow")

xset("alufunction",6)
xset("thickness",2)
t0=0; dt=0.05; tmax=15;
t=t0:dt:tmax;
oldx0=10*xmax; oldy0=10*ymax;
dx=0.1; dy=0.1;
rtol=0.0001; atol=rtol;
while (%t)
  [b,x0,y0]=xclick();
  if b==2 then break end;
  if b==0 & x0>=xmin & x0<=xmax & y0>=ymin & y0<=ymax then
    sol=ode([x0;y0],t0,t,rtol,atol,f);
    plot2d(sol(1,:),sol(2,:),5,"000")
    xset("wshow")
    rep=[x0,y0,1];  
    while rep(3)<>0 then
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
