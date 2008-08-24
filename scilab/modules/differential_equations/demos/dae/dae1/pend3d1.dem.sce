//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

exec(SCI+"/modules/differential_equations/demos/dae/dae1/pendule3d.sci");

m = 1;
g = 10;
l = 1;

theta0 = 0.2;
phi0   = %pi/4;

x0=l*[sin(phi0)*cos(theta0);cos(phi0)*cos(theta0);sin(theta0)];

// index 1

u0      = [1;-1;0];
function  r=ff(l),r=ggpp(x0,u0,l);endfunction
lambda0 = fsolve(0,ff);
ud0     = -2*lambda0/m*x0-[0;0;g];
y0      = [x0;u0;lambda0];
yd0     = [u0;ud0;0];
t0      = 0;
T       = t0:0.01:15;

info = list([],0,[],[],[],0,0);
atol = [0.0001;0.0001;0.0001;0.001;0.001;0.001;1];
rtol = atol;

y1   = dassl([y0,yd0],t0,T,rtol,atol,index1,info);

x1=y1(2:4,:);

norm(x1(:,$),2)

my_handle = scf(100001);
clf(my_handle,"reset");
demo_viewCode("pend3d1.dem.sce");

drawlater()
title(_("spherical pendulum, index 1"),"fontsize",3)
xpoly([],[]);
p                 = gce();
a                 = gca();
a.view            = '3d';
a.box             = 'on';
a.grid            =color('lightgray')*ones(1,3);
a.rotation_angles = [35 45];
a.axes_visible    = "on";
a.data_bounds     = [min(x1(1,:)),min(x1(2,:)),min(x1(3,:))
		     max(x1(1,:)),max(x1(2,:)),max(x1(3,:))];
p.data=[x1(1,1) x1(2,1),x1(3,1)];
drawnow();

step=10;
for k=1:step:(size(x1,2)-step)
  p.data=[p.data
	  x1(:,k:(k+step-1))'];
end

