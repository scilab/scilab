function demo_pendulum()
  demo_help demo_pendulum
  npend_build_and_load() ;
  n=np();
  r=ones(1,n);
  m=ones(1,n);
  j=ones(1,n);
  g=9.81;
  
  y0=0*ones(2*n,1);tt=0:0.05:10;
  yt=ode(y0,0,tt,'npend');
  draw_chain_from_angles(yt(1:$/2,:),r,1);
  xdel()
endfunction




function []=npend_build_and_load() 
// since this demo can be run by someone 
// who has no write access in this directory 
// we use TMPDIR 

  if ~c_link('npend') then
    curdir = getcwd(); 
    chdir(TMPDIR); 
    fcode=mgetl(curdir+'/npend/Maple/dlslv.f');mputl(fcode,'dlslv.f')
    fcode=mgetl(curdir+'/npend/Maple/ener.f');mputl(fcode,'ener.f')
    fcode=mgetl(curdir+'/npend/Maple/np.f');mputl(fcode,'np.f')
    fcode=mgetl(curdir+'/npend/Maple/npend.f');mputl(fcode,'npend.f')
    files = ['npend.o','np.o','ener.o','dlslv.o' ];
    ilib_for_link(['npend';'np';'ener'],files,[],"f");
    exec loader.sce 
    chdir(curdir) 
  end
endfunction 

function [n]=np()
// Return the size  of the Fortran pendulum 
// Copyright ENPC
  n=1;
  n=fort('np',n,1,'i','sort',1);
endfunction 

function [ydot]=npend ( t, th)
// Fortran version 
//      data r  / 1.0, 1.0, 1.0, 1.0 /
//      data m  / 1.0, 1.0, 1.0, 1.0 /
//      data j  / 0.3, 0.3, 0.3, 0.3 /
  ydot=ones(6,1)
  ydot=fort('npend',3,1,'i',t,2,'d',th,3,'d',ydot,4,'d','sort',4);
endfunction 

function [E]=ener( th)
  E=0.0;
  E=fort('ener',th,1,'d',E,2,'d','sort',2);
endfunction 

function draw_chain_from_angles(a,r,job)
// a the angles , a(i,j) is the angle of node i a time t(j)
// r the segments half length
  if argn(2)<3 then job=0,end
  n2=size(a,2);
  // build the links positions
  x=[0*ones(1,n2);cumsum(2*diag(r)*cos(a),1)];
  y=[0*ones(1,n2);cumsum(2*diag(r)*sin(a),1)];
  draw_chain_from_coordinates(x,y,job)
endfunction

function draw_chain_from_coordinates(x,y,job)
// x,y the coordinates , 
//     x(i,j), y(i,j) is the coordinate of node i a time t(j)
// r the segments half length
  if argn(2)<3 then job=0,end
  [n1,n2]=size(x);
 
  //set the frame
  xbasc();
  f = gcf() ;
  f.pixmap = 'on' ;
  SetPosition() ;
  set figure_style new;a=gca()
  a.data_bounds=2*[-n1,-n1;n1,0];
  a.isoview = 'on' ;
  
  //create one polyline and one polymark with the initial position
  drawlater() //not to see the intermediate graphic steps
  xsegs([x(1:$-1,1)';x(2:$,1)'],[y(1:$-1,1)';y(2:$,1)'],1:n1-1)
  p=gce();p.thickness=4;
  xpoly(x(:,1),y(:,1),'lines');
  p1=gce();p1.mark_style=3;p1.mark_size=1;
  if job==1 then
    //bound trajectory
     xpoly(x($,1)*ones(2,1),y($,1)*ones(2,1),'lines');
     t=gce();t.line_style=2;
  end
  drawnow()
  ind=[1;(2:n1-1)'.*.ones(2,1);n1]
  realtimeinit(0.1)
  for j=1:n2,
    realtime(j) //to slow down the drawing
    drawlater() 
    // update chain coordinates
    p1.data = [x(:,j),y(:,j)]; 
    p.data = [x(ind,j),y(ind,j)]; 
    // add a trajectory point
    if job==1 then t.data=[t.data;[x($,j),y($,j)]],end
    drawnow()
    show_pixmap() ;
  end
endfunction

