// Copyright INRIA

mode(1)
oldln=lines();
lines(0)

deff("yprim=f(t,y)","yprim=y^2-t")

tmin=-3; tmax=5; ymin=-3; ymax=3;

deff("z=g(t,y)","z=[y-ymin;y+ymax]")

t=tmin:1:tmax; y=ymin:1:ymax;

xbasc()
xset("pixmap",1)
nt=size(t,"*"); ny=size(y,"*");
fx=ones(nt,ny);
fy=feval(t,y,f);
champ(t,y,fx,fy);
xset("font",2,12) 
xtitle([""],["t"],["y"])
xset("wshow")

xset("alufunction",6)
xset("thickness",2)
oldt0=10*tmax; oldy0=10*ymax;

dt=0.1; dy=0.1;
while(%t)
  [b,t0,y0]=xclick();
  if b==2 then break end;
  if b==0 & tmin<t0 & t0<tmax & ymin<y0 & y0<ymax then
    t1=t0:0.1:tmax;
    [sol1,rd]=ode("root",y0,t0,t1,1.d-2,1.d-4,f,2,g);
    plot2d(t1(1:size(sol1,"*")),sol1,5,"000")
    t2=t0:-0.1:tmin;
    [sol2,rd]=ode("root",y0,t0,t2,1.d-2,1.d-4,f,2,g);
    plot2d(t2(1:size(sol2,"*")),sol2,5,"000")
    xset("wshow")
    rep=[t0,y0,1];
    while rep(3)<>0 then
      rep=xgetmouse(0);
      t0=rep(1); y0=rep(2);
      if (tmin<t0 & t0<tmax & ymin<y0 & y0<ymax) &..
	(abs(t0-oldt0)>=dt | abs(y0-oldy0)>=dy) then
	plot2d(t1(1:size(sol1,"*")),sol1,5,"000")
	plot2d(t2(1:size(sol2,"*")),sol2,5,"000")
	t1=t0:0.1:tmax; 
	[sol1,rd]=ode("root",y0,t0,t1,1.d-2,1.d-4,f,2,g);
	plot2d(t1(1:size(sol1,"*")),sol1,5,"000")
	t2=t0:-0.1:tmin;
	[sol2,rd]=ode("root",y0,t0,t2,1.d-2,1.d-4,f,2,g);
	plot2d(t2(1:size(sol2,"*")),sol2,5,"000")
	xset("wshow")
	oldt0=t0; oldy0=y0;
      end
    end
    xset("alufunction",1)
    t1=oldt0:0.1:tmax; 
    plot2d(t1(1:size(sol1,"*")),sol1,5,"000")
    t2=oldt0:-0.1:tmin;
    plot2d(t2(1:size(sol2,"*")),sol2,5,"000")
    xset("wshow")
    xset("alufunction",6)
  end
end
xset("pixmap",0)
xset("default")
lines(oldln(1))
