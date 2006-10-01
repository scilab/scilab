function [pt,dx,dz,dt]=acoustic(vel,tf,fc,spos,dx,dz,dt)
//[pt[,dx,dz,dt]]=acoustic(vel,tf,fc,spos[,dx,dz][,dt])
//////////////////////////////////////////////////////
//                                                  //
//  Scilab program to simulate forward propagation  //
//  of acoustic waves with absorbing boundary       //
//  conditions where p is the wavefield, s is the   //
//  source, and v is the velocity field:            //
//                                                  //
//                      2  __ 2                     //
//             P   =  v    \/   P + S               // 
//              tt                                  //
//                                                  //
//////////////////////////////////////////////////////
// vel  :Velocity distribution (matrix which represents
//      :distribution of velocity as function of offset x 
//      :and depth z.  Increasing offset and depth goes
//      :as the increasing indices of the matrix vel).
// tf   :Final time (initial time is zero)
// fc   :Center frequency of wavelet (derivative of gaussian)
// spos :source postion which is a 2-vector of integers 
//      :where 1<=spos(1)<=nr, 1<=spos(2)<=nc 
//      :and [nr,nc]=size(vel).
// dx   :Sampling step in offset 
// dz   :Sampling step in depth
// dt   :Sampling step in time.  
// ntbl :Name table containing names of all the files 
//      :created containing data
//
//The parameters dx, dz, and dt are optional and are
//calculated automatically, when not specified, to satisfy
//stability conditions and to impose an acceptable level
//of numerical dispersion.
//
//!
//author: C. Bunks     date: 29-Oct-90
// Copyright INRIA

[lhs,rhs]=argn(0);
   lines(0);

//velocity parameters

   [nr,nc]=size(vel);
   vmax=maxi(vel);vmin=mini(vel);

//default check

   if rhs==4 then,//auto calculation of dx, dz, and dt
      dx=vmin/(16*fc);
      dz=dx;
      dmin=mini([dx,dz]);dmax=maxi([dx,dz]);   
      dt=.95*dmin/(vmax*sqrt(2));//stability condition 
   end,
   if rhs==5 then,//auto calculation of dx and dz
      dx=vmin/(16*fc);
      dz=dx;
      dmin=mini([dx,dz]);dmax=maxi([dx,dz]);   
   end,
   dmin=mini([dx,dz]);dmax=maxi([dx,dz]);   
   if rhs==6 then,//auto calculation of dt
      dt=.95*dmin/(vmax*sqrt(2));//stability condition 
   end,

//inform user of default choices

   write(%io(2),' '),
   write(%io(2),' CHOICES OF DX, DZ, AND DT:'),
   print(6,[dx,dz,dt]),
   write(%io(2),' '),

//ERROR CHECKING
//source location

   eflag='on';
   if 1<=spos(1) then, if spos(1)<=nr then,
   if 1<=spos(2) then, if spos(2)<=nc then,
      eflag='off';
   end,end,
   end,end,
   if eflag=='on' then,
      write(%io(2),'                               '),
      write(%io(2),'*************ERROR*************');
      write(%io(2),'                               '),
      write(%io(2),' SOURCE POSITION OUT OF BOUNDS ');
      write(%io(2),'                               '),
      write(%io(2),'*************ERROR*************');
      write(%io(2),'                               '),
      error('sim.bas'),
   end,

//space discretization

   if dmax>vmin/(15*fc) then,
      write(%io(2),'                                       '),
      write(%io(2),'****************WARNING****************');
      write(%io(2),'                                       '),
      write(%io(2),' NUMERICAL DISPERSION LIKELY           '),
      write(%io(2),' VARIABLES SHOULD SATISFY:             '),
      write(%io(2),'                                       '),
      write(%io(2),'     max([dx,dz]) < vmin/(15*fc)       '),
      write(%io(2),'                                       '),
      write(%io(2),'****************WARNING****************');
      write(%io(2),'                                       '),
   end,

//time discretization and stability check
//           v*dt/dx < 1/sqrt(2)

   if (vmax*dt/dmin)>(1/sqrt(2)) then,
      write(%io(2),'                                       '),
      write(%io(2),'*****************ERROR*****************');
      write(%io(2),'                                       '),
      write(%io(2),' UNSTABLE CHOICES: vel, dt, dx, and dz '),
      write(%io(2),' VARIABLES MUST SATISFY:               '),
      write(%io(2),'                                       '),
      write(%io(2),'    max(vel)*dt*sqrt(2) < min(dx,dz)   '),
      write(%io(2),'                                       '),
      write(%io(2),'*****************ERROR*****************');
      write(%io(2),'                                       '),
      error('sim.bas'),
   end,
   t=0:dt:tf;

//pre-calculation

   v2dt=(dt*dt)*(vel.*vel);
   zr=0*ones(1,nc);zc=0*ones(nr,1);

//integrate forward 

//   unit=file('open','pt.dat','unknown','unformatted');
   pt=[];
   pkm1=0*ones(vel);pkm2=0*ones(vel);//initial conditions
   for tk=t,
      src=shot(tk,fc);
      pk=integrate(tk,pkm1,pkm2,src,spos);
      pt=[pt;pk];
//      writb(unit,pk);
      pkm2=pkm1;pkm1=pk;
   end,
//   file('rewind',unit);
//   pt=readb(unit,nr*maxi(size(t)),nc);
//   file('close',unit);


function [utp1]=integrate(t,ut,utm1,src,spos)
//[utp1]=integrate(t,ut,utm1,src,spos)
//compute second order time update of acoustic wave equation
//(with absorbing boundaries)
// t     :current time
// ut    :wavefield at time t
// utm1  :wavefield at time t-dt
// src   :source value at time t
// spos  :source position
// utp1  :wavefield at time t+dt
//
//!
//author: C. Bunks     date: 29-OCT-90
// Copyright INRIA

   write(%io(2),'t='+string(t));

//calculate laplacian in the interior of the medium

   utp1= 2*ut...
        -utm1...
        +v2dt.*(([ut(:,2:nc) zc]+[zc ut(:,1:nc-1)]-2*ut)/dx**2...
               +([ut(2:nr,:);zr]+[zr;ut(1:nr-1,:)]-2*ut)/dz**2);

//calculate boundary conditions on edges  (fix velocities)
//(see paper by Reynolds, Geophysics, v. 43, 1978, pp1099-1110)

//right side boundary
   ua=ut(:,nc-1);ub=ut(:,nc);uc=utm1(:,nc-2);ud=utm1(:,nc-1);
   utp1(:,nc)=(ua+ub-ud)-(dt/dx)*vel(:,nc).*(ub-ua+uc-ud);

//left side boundary
   ua=ut(:,2);ub=ut(:,1);uc=utm1(:,3);ud=utm1(:,2);
   utp1(:,1)=(ua+ub-ud)-(dt/dx)*vel(:,1).*(ub-ua+uc-ud);

//bottom boundary
   ua=ut(nr-1,:);ub=ut(nr,:);uc=utm1(nr-2,:);ud=utm1(nr-1,:);   
   utp1(nr,:)=(ua+ub-ud)-(dt/dz)*vel(nr,:).*(ub-ua-ud+uc);

//top boundary (absorbing or free)
   ua=ut(2,:);ub=ut(1,:);uc=utm1(3,:);ud=utm1(2,:);   
   utp1(1,:)=(ua+ub-ud)-(dt/dz)*vel(1,:).*(ub-ua-ud+uc);

//calculate boundary conditions at corners
   utp1(1,1)=ut(1,1)+((dt/dx)*vel(1,1))*(ut(2,2)-ut(1,1));
   utp1(1,nc)=ut(1,nc)+((dt/dx)*vel(1,nc))*(ut(2,nc-1)-ut(1,nc));
   utp1(nr,1)=ut(nr,1)+((dt/dx)*vel(nr,1))*(ut(nr-1,2)-ut(nr,1));
   utp1(nr,nc)=ut(nr,nc)+((dt/dx)*vel(nr,nc))*(ut(nr-1,nc-1)-ut(nr,nc));

//add in source
   sz=spos(1);sx=spos(2);
   utp1(sz,sx)=utp1(sz,sx)+dt**2*src;

function [dg]=shot(t,fc)
//[dg]=shot(t,fc)
//calculate shot values as a function of time
//as the derivative of a gaussian:
//
//   g=exp(-(t-m)**2/(2*sigma**2))
//   dg=-g*(t-m)/sigma**2
//
//Center frequency for the derivative of the gaussian is
//at fc=1/(2*m). The ratio of the minimum velocity (vm) 
//to center frequency (fc) (i.e., the smallest spatial wavelength)
//should be between 10 and 20 times greater than the largest 
//spatial discretization.  Here we take the factor to be 15:
//
//              vm/fc = 2*m*vm > 15*maxi(dx,dz)
//or
//                  m = 7.5*maxi(dx,dz)/mini(vel)
//
// t  :current time
// fc :center frequency of the wavelet
// dg :derivative of a gaussian at time t
//
//!
//author: C. Bunks     date: 29-OCT-90
// Copyright INRIA

   m=1/(2*fc);
   sig=m/4;
   sig2=sig**2;
   g=exp(-(t-m)**2/(2*sig2))/sqrt(2*%pi*sig2);
   dg=-g*(t-m)/sig2;


function [pt]=get_data(ntbl,entry)
//Search for a data file written on disk
// ntbl  :table of file names (first two entries give 
//       :data dimensions)
// entry :integer giving the entry in the table-2
// pt    :returned data file
// Copyright INRIA
   ts=maxi(size(ntbl));
   nr=evstr(ntbl(1));
   nc=evstr(ntbl(2));
   if entry<=ts-2 then,
      filename=ntbl(entry+2);
      unit=file('open',filename,'unknown');
      pt=read(unit,nr,nc);
      file('close',unit);
      plot2d(pt,'agc',[0,%pi/4],'x');
   else,
      write(%io(2),' '),
      write(%io(2),' Table Entry to Large...Max Value='+string(ts-2)),
      write(%io(2),' '),
   end,



