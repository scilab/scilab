// Copyright INRIA

mode(1)
oldln=lines();
lines(0)
x_message(["Examples of ODE''s in 1 dimension";" ";
           "A trajectory is plotted by clicking on the";
           "  LEFT button of the mouse."
           "  The trajectory is updated as you move the mouse.";
           "  To fix the trajectory, click again on the LEFT button.";
           "You can start over by clicking on the LEFT button again";
           "  or stop everything by clicking on the RIGHT button."]);

deff("yprim=f(t,y)","yprim=y^2-t")

tmin=-3; tmax=5; ymin=-3; ymax=3;

deff("z=g(t,y)","z=[y-ymin;y+ymax]")

t=tmin:1:tmax; y=ymin:1:ymax;

clf();
toolbar(0,'off');
nt=size(t,"*"); ny=size(y,"*");
fx=ones(nt,ny);
fy=feval(t,y,f);
champ(t,y,fx,fy);

a=gca();
a.x_label.text="y1",a.x_label.font_size=3;
a.y_label.text="y2",a.y_label.font_size=3;
oldt0=10*tmax; oldy0=10*ymax;
fig=gcf();fig.pixmap='on';
show_pixmap();
dt=0.1; dy=0.1;
while(%t)
  [b,t0,y0]=xclick();
  if or(b==[2 5]) then break,end;
  if or(b==[0 3])& tmin<t0 & t0<tmax & ymin<y0 & y0<ymax then
    t1=t0:0.1:tmax;
    [sol1,rd]=ode("root",y0,t0,t1,1.d-2,1.d-4,f,2,g);
    xpoly(t1(1:size(sol1,"*"))',sol1');
    p1=gce();p1.thickness=2;p1.foreground=5;


    t2=t0:-0.1:tmin;
    [sol2,rd]=ode("root",y0,t0,t2,1.d-2,1.d-4,f,2,g);
    xpoly(t2(1:size(sol2,"*"))',sol2');
    
    p2=gce();p2.thickness=2;p2.foreground=5;
    show_pixmap()
    rep=[t0,y0,-1];
    while rep(3)==-1 then
      rep=xgetmouse(0);
      t0=rep(1); y0=rep(2);
      if (tmin<t0 & t0<tmax & ymin<y0 & y0<ymax) &..
	(abs(t0-oldt0)>=dt | abs(y0-oldy0)>=dy) then
	t1=t0:0.1:tmax; 
	[sol1,rd]=ode("root",y0,t0,t1,1.d-2,1.d-4,f,2,g);
	t2=t0:-0.1:tmin;
	[sol2,rd]=ode("root",y0,t0,t2,1.d-2,1.d-4,f,2,g);
	p1.data=[t1(1:size(sol1,"*"))' sol1'];
	p2.data=[t2(1:size(sol2,"*"))' sol2'];
	show_pixmap()
	oldt0=t0; oldy0=y0;
      end
    end
  end
end
fig.pixmap='off';
lines(oldln(1))
