// Copyright INRIA

function [res,ires]=pendg(t,y,ydot)
x=y(1:3); u=y(4:6); lambda=y(7);
xp=ydot(1:3); up=ydot(4:6);
res(1:3)=xp-u;
res(4)=(M+m)*up(1)+m*l*cos(x(3))*up(3)-m*l*sin(x(3))*u(3)^2..
+lambda*fx(x(1),x(2))+k*u(1);
res(5)=(M+m)*up(2)+m*l*sin(x(3))*up(3)+m*l*cos(x(3))*u(3)^2..
+(M+m)*g+lambda*fy(x(1),x(2))+k*u(2);
res(6)=m*l*cos(x(3))*up(1)+m*l*sin(x(3))*up(2)+m*l^2*up(3)..
+m*g*sin(x(3));
res(7)=-(fx(x(1),x(2))*u(1)+fy(x(1),x(2))*u(2));
ires=0;
endfunction

function  H=build_sliding_pendulum ()
//build the sliding pendulum figure and graphic objects, 
//return the handle on moving parts
  xbasc();
  //set the figure
  set figure_style new;xselect(),
  f=gcf();f.pixmap='on';a=gca();drawlater()
  f.figure_size=[610,676] //mode isoview does not work
  
  xmin=-1.5; xmax=1.5; ymin=-1.1; ymax=2.25;
  a.data_bounds=[xmin ymin;xmax ymax]
  //the framework
  xrect(xmin,ymax,xmax-xmin,ymax-ymin)
  //the curve
  vx=[xmin:0.1:xmax]'; vy=vx^2;
  xpoly(vx,vy,"lines")
  c=gce();c.foreground=5;
  //the pendulum segment
  x=0; y=0; teta=0;
  xp=x+l*sin(teta); yp=y-l*cos(teta);
  r=0.05 // the bullet half diameter
  xp1=x+(l-r)*sin(teta); yp1=y-(l-r)*cos(teta);
  xpoly([x;xp1],[y;yp1],"lines")
  p=gce();p.thickness=2;
  //the pendulum bullet
  xfarc(xp-r,yp+r,2*r,2*r,0,360*64)
  b=gce()
  H=glue([p,b]) //return the handle on segment and bullet
endfunction

function  draw_sliding_pendulum (H,state)
//draw the pendulum in a given state  
  x=state(1); y=state(2); teta=state(3); 
  // bullet half diameter
  b = H.children(1);r=b.data(3)/2

  xp=x+l*sin(teta); yp=y-l*cos(teta);
  xp1=x+(l-r)*sin(teta); yp1=y-(l-r)*cos(teta);
  drawlater()
  p = H.children(2);p.data=[x, y; xp1, yp1];
  b = H.children(1); b.data=[xp-r,yp+r,2*r,2*r,0,360*64];
  drawnow();
  show_pixmap()
endfunction
