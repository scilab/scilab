function []=show(xx,t,p)
//[]=show(xx,t,p)
// Just show the wheel evolution 
// t ans p are the spherical angles of the observation point
// given in radian
// f_name is the function name to use for display 
// can be wheel2
//                     .    .   .
// xx= [theta,phi,psi,teta,phi,psi,x,y]
// !
// Copyright ENPC 
  [lhs,rhs]=argn(0)
  if rhs <= 2 , p=%pi/3;end
  if rhs <= 2 , t=%pi/3;end
  if rhs <= 3 , slowflag=0;end
  nstep=1;r1=1.0;  //change nstep for postscript
  [nnr,nn]=size(xx);
  time=1:nstep:nn;
  //------------ generation of the wheel
  //[1] a set of points on the wheel
  //the first one is the contact point 
  //the coordinates are given in the (u,v,w) space 
  // as a matrix xu(:,t) ....
  nnn=24
  l=(1/nnn)*( 2*%pi)*(0:nnn)'
  nu=prod(size(l));
  xu=r1*cos(l).*.ones(time);
  yu=r1*sin(l).*.ones(time);
  zu=0*ones(l).*.ones(time);

  //[2] Adding rays ( they are moving with time in the (u,v,w) space 
  //turning around w with angle psi
  //in xr,yr,zr : four moving points plus the center 
  l= ones(4,1).*.xx(3,time) + [0;%pi/2;%pi;3*%pi/2].*.ones(time);
  xr=r1*cos(l).*.[0;1]
  yr=r1*sin(l).*.[0;1]
  zr=0*ones(l).*.[0;1]
  [nr,pr]=size(xr);  

  //[3] using wheelg to transform these vectors in the (x,y,z) space 
  [xu,yu,zu]=wheelg(nu,prod(size(time)),xu,yu,zu,xx(:,time));
  [xr,yr,zr]=wheelg(nr,prod(size(time)),xr,yr,zr,xx(:,time));

  xmin=mini(xu);xmax=maxi(xu);
  ymin=mini(yu);ymax=maxi(yu);
  zmin=mini(zu);zmax=maxi(zu);
  rect=[xmin,xmax,ymin,ymax,zmin,zmax]

  //[4] plotting frame
  t=t*180/%pi,p=p*180/%pi,
  plot3d([xmin,xmax],[ymin,ymax],zmin*ones(2,2),t,p," ",[0,1,0],rect)

  //[4'] I want to plot the rays with xpoly so i first use geom3d
  [xr,yr]=geom3d(xr,yr,zr);

  //[4''] Same to use xpoly so i first use geom3d
  [xu,yu]=geom3d(xu,yu,zu);

  //[5] animation 
  xset("alufunction",6)
  [n1,n2]=size(xu);
  get_wheel_rti(%t);
  if ~isdef('wheel_rti') then wheel_rti=0.03;end 
  realtimeinit(wheel_rti);
  realtime(0)
  for i=1:1:n2-1,
    wheeld(i);
    realtime(i);
    wheeld(i);
    ww=i:i+1;
    xpoly(xu(1,ww)',yu(1,ww)',"lines");
  end
  wheeld(n2-1);
  xset("alufunction",3);
  [wheel_rti]=resume(wheel_rti);  
endfunction 

function []=wheeld(i)
// Copyright ENPC
  xpoly(xu(:,i),yu(:,i),"lines");
  xpoly(matrix(xr(:,i),2,4),matrix(yr(:,i),2,4),"lines");
endfunction 


function []=wheeld(i)
// Copyright ENPC
  xfpoly(xu(:,i),yu(:,i),1);
  xpoly(matrix(xr(:,i),2,4),matrix(yr(:,i),2,4),"lines");
endfunction 


function [xxu,yyu,zzu]=wheelgf(n,t,xu,yu,zu,xx)
// Copyright ENPC
//
  [xxu,yyu,zzu]=fort('wheelg',n,1,'i',t,2,'i',xu,3,'d',yu,4,'d',zu,5,'d',...
		     xx,6,'d','sort',3,4,5);
endfunction 

function [y]=test_wheel(n,t,x)
// Copyright ENPC
//
  y=x
  [y]=fort('wheel',n,1,'i',t,2,'d',x,3,'d',y,4,'d','sort',4);
endfunction 

function [xxu,yyu,zzu]=wheelgs(n,t,xu,yu,zu,xx)
// Copyright ENPC
// slower version without dynamic link 
  r=1.0
  [n,p]=size(xu);
  xxu=xu;
  yyu=yu;
  zzu=zu;
  for i1=1:n;
    cs2 = cos(xx(2,:))
    cs1 = cos(xx(1,:))
    si1 = sin(xx(1,:))
    si2 = sin(xx(2,:))
    xxu(i1,:) =xx(7,:)+r*(cs2.*cs1.*xu(i1,:)-si1.*yu(i1,:)+si2.*cs1.*zu(i1,:));
    yyu(i1,:) =xx(8,:)+r*(cs2.*si1.*xu(i1,:)+cs1.*yu(i1,:)+si2.*si1.*zu(i1,:));
    zzu(i1,:) =	   r*si2 +r*( -si2.*xu(i1,:)+cs2.*zu(i1,:));
  end
endfunction 

function []=wheel_build_and_load() 
// since this demo can be run by someone 
// who has no write access in this directory 
// we use TMPDIR 
  if ~c_link('wheel') then
    cd = getcwd(); 
    chdir(TMPDIR); 
    path='SCI/demos/simulation/wheel2/Maple/'
    fcode=mgetl(path+'/dlslv.f');mputl(fcode,'dlslv.f')
    fcode=mgetl(path+'/wheel.f');mputl(fcode,'wheel.f')
    fcode=mgetl(path+'/wheelg.f');mputl(fcode,'wheelg.f')
    files = ['wheel.o','wheelg.o','dlslv.o' ];
    ilib_for_link(['wheel';'wheelg'],files,[],"f");
    exec loader.sce 
    chdir(cd) 
  end
endfunction 

function get_wheel_rti(d_mode) 
  data_rti=['timeunit for realtimeinit','wheel_rti','0.02'];
  [d_r,d_c]=size(data_rti);
  for i=1:d_r, 
    if isdef(data_rti(i,2)) then 
      data_rti(i,3)= string(evstr(data_rti(i,2)));
    else 
      execstr(data_rti(i,2)+'='+data_rti(i,3));
    end
  end
  
  if d_mode then 
    ddd= data_rti(:,1);
    data_rti_mdial=x_mdialog('time unit for graphics',ddd, data_rti(:,3));
    
    if data_rti_mdial <> [] then 
      for i=1:d_r, 
	execstr(data_rti(i,2)+'='+data_rti_mdial(i));
      end
    end
  end
  [wheel_rti]=resume(wheel_rti);
endfunction 




