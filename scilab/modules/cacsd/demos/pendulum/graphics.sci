 function P=initialize_display(xg,teta)
   clf();a=gca();a.isoview='on';
   f=gcf();f.pixmap='on'
   a.data_bounds=[-0.4 -0.2;0.4 0.3];
   y1=0;lb=l;hc=0.05;lc=0.1;teta=.25;r=0.013
   P=build_pendulum([xg,y1],[lc,hc,lb,teta,r])
   xsegs([-0.4 0.4],[y1-2*r y1-2*r]);
   show_pixmap()
 endfunction


function [P]=dpnd()
//dpnd() scheme of experiment
//!
  set figure_style new
  clf();a=gca();a.isoview='on';
  f = gcf() ; f.figure_size = [640,480];
  a.data_bounds=[0 0;100 100];
  xg=40;y1=25;lb=40;hc=10;lc=20;teta=.25;r=2.5;
  P=build_pendulum([xg,y1],[lc,hc,lb,teta,r])
 
  //the floor
  xarrows([10 90],[y1-5 y1-5],50); xstring(90,y1,'x')
  
  // the force
  yg=y1+hc/2, 
  x2=xg+lc/2;
  xarrows([x2 x2+10],[yg yg],50);
  xstring(x2+20,yg,'u (force)',0,0);
  
  // the vertical
  y2=y1+hc;
  xsegs([xg xg],[y2 y2+lb]);e=gce();e.line_style=2;
  
  // the angle teta
  xstring(xg+lb*sin(teta)/2,y2+lb*cos(teta),'a',0,0);
  e=gce();e.font_size=3;
  
  //the différential equations
  xstring(5,-6,['a'''' = (-sin(a)*cos(a)*(m/(m+M))*a''^2 + 2/(mb*l)*(sin(a)*m*g - qm*cos(a)*u))/d'
		'x'''' =  (u+m*(l/2)*(sin(a)a''^2-cos(a)*a''''))/(m+M);'
		'm: weight of the pendulum'
		'M: weight of the cart'
		'l: length of the pendulum'])
endfunction

function P=build_pendulum(o,params)
  xg=o(1)
  y1=o(2)
  lc=params(1) //width of the cart
  hc=params(2) //height of the cart
  lb=params(3) //length of the pendulum
  teta=params(4) //angle  of the pendulum
  r=params(5) //radius of wires
  y2=y1+hc;
  x2=xg+lc/2;
  x1=xg-lc/2;
  
  //cart
  xrect([xg-lc/2,y1+hc,lc,hc]);e1=gce();
  xfarcs([x1+lc/10-r;y1;2*r;2*r;0;360*64],1);e2=gce();
  xfarcs([x2-2*r+lc/10-r;y1;2*r;2*r;0;360*64],1);e3=gce();

  //pendulum
  xsegs([xg,xg+lb*sin(teta)],[y2,y2+lb*cos(teta)]),
  e4=gce();e4.thickness=2;
  P=glue([e4 e3 e2 e1])
  P.user_data=[xg,lb]
endfunction
 
function P=set_pendulum(P,x,theta)
  p=P.user_data
  xg=p(1);lb=p(2);
  drawlater()
  //translation
  e=P.children(1);e.data(1)=e.data(1)+x-xg;
  e=P.children(2).children;e.data(1)=e.data(1)+x-xg;
  e=P.children(3).children;e.data(1)=e.data(1)+x-xg;
  e=P.children(4);e.data(:,1)=e.data(:,1)+x-xg;
  
   //change the pendulum angle
  e.data(2,:)=e.data(1,:)+[lb*sin(theta) lb*cos(theta)];
  P.user_data(1)=x
  drawnow()
  show_pixmap()
endfunction


function draw1()
  f=gcf();f.figure_position=[10 10];xselect()
  clf();f.background=color('gray');
  f.pixmap='on';drawlater()
  f.figure_size=[850,650];
  y=y(:,1:70);  n=size(y,2);
  a1=gca();sca(a1);
  a1.axes_bounds=[0 0 0.5 0.5];
  a1.data_bounds=[1,min(y(1,:));n max(y(1,:))];
  a1.axes_visible='on';
  a1.x_label.text='t';
  a1.y_label.text='position';
  p1=xpoly(1,y(1,1));p1=gce();

  a2=newaxes();sca(a2);
  a2.axes_bounds=[0.5,0,0.5,0.5];
  a2.data_bounds=[1,min(y(2,:));n max(y(2,:))];
  a2.axes_visible='on';
  a2.x_label.text='t';
  a2.y_label.text='theta';
  xpoly(1,y(2,1));;p2=gce();

  a3=newaxes();
  a3.axes_bounds= [0,0.5,1,0.5];
  a3.isoview='on';
  a3.data_bounds=[-0.4 -0.1;0.4 0.4];
  y1=0;lb=l;hc=0.05;lc=0.1;teta=100*y(2,1);r=0.013;xg=100*y(1,1);
  sca(a3);
  P=build_pendulum([xg,y1],[lc,hc,lb,teta,r])
  xsegs([-0.4 0.4],[y1-2*r y1-2*r]);

  show_pixmap();drawnow()


  for k=1:size(y,2)
    drawlater()
    xx=100*y(1,k);tt=100*y(2,k);
    p1.data=[p1.data;k,y(1,k)];
    p2.data=[p2.data;k,y(2,k)];
    P=set_pendulum(P,xx,tt);
  end
  f.pixmap='off'
endfunction

function draw2()
  f=gcf();f.figure_position=[10 10];xselect()
  clf();f.background=color('gray');
  f.pixmap='on';drawlater()
  f.figure_size=[850,650];
  
  yd=yd(:,1:100);  n=size(yd,2);
  c = kr*yd(5:8,:) //control
  theta = yd(3,:) //angle
  thetaE= yd(7,:) // estimated angle
  x = yd(1,:)
  
  a1=gca();sca(a1);
  a1.axes_bounds=[0 0 0.5 0.5];
  a1.data_bounds=[min(t1),min(c);t1(n) max(c)];
  a1.axes_visible='on';
  a1.x_label.text='time';
  a1.y_label.text='Control (u)';
  p1=xpoly(t1(1),c(1));p1=gce();

  a2=newaxes();sca(a2);
  a2.axes_bounds=[0.5,0,0.5,0.5];
  a2.data_bounds=[t1(1),min([theta thetaE]);t1(n) max([theta thetaE])];
  a2.axes_visible='on';
  a2.x_label.text='t';
  a2.y_label.text='theta';
  xpoly(t1(1),theta(1));p2=gce();
  xpoly(t1(1),thetaE(1));p3=gce();p3.line_style=2;

  a3=newaxes();
  a3.axes_bounds= [0,0.5,1,0.5];
  a3.isoview='on';
  a3.data_bounds=[-0.4 -0.1;0.4 0.4];
  y1=0;lb=l;hc=0.05;lc=0.1;;r=0.013;
  sca(a3);
  P=build_pendulum([100*x(1),y1],[lc,hc,lb,100*theta(1),r])
  xsegs([-0.4 0.4],[y1-2*r y1-2*r]);

  show_pixmap();drawnow()

  for k=1:n
    drawlater()
    xx=x(k);tt=theta(k);
    p1.data=[p1.data;t1(k),c(k)];
    p2.data=[p2.data;t1(k),theta(k)];
    p3.data=[p3.data;t1(k),thetaE(k)];
    P=set_pendulum(P,xx,tt);
  end
  f.pixmap='off'
  
endfunction
