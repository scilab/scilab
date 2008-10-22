//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

thispath = get_absolute_file_path("wheel2.dem.sce");

my_handle = scf(100001);
clf(my_handle,"reset");

exec(thispath+"show.sci");

if ~(haveacompiler()) then
	x_message(["Scilab doesn''t find a C compiler","This demo is disabled"]);
	return;
end

mode(0);
wheel_build_and_load();
wheelg = wheelgf;
tmin   = 0.0;
tmax   = 15;
nn     = 300;
times  = (0:(nn-1));
times  = tmax*times/(nn-1) +tmin*((nn-1)*ones(times)-times);

// Initial conditions
// =========================================================================
x0=[0;             // theta
	%pi/2+0.1;     // phi
	0;             // psi
	5.0;           // Dtheta
	0.0;           // Dphi
	4.0;           // Dpsi
	0;             // x
	0];            // y

// Simulation
// =========================================================================

x=ode(x0,tmin,times,"wheel");

clf(my_handle,"reset");
show(x);

ystr = [ 'phi';'theta';'psi';'Dpsi';'Dtheta';'Dpsi';'x';'y'];
flag = 2;

while flag==2, [n1,n2]=size(x);
	flag=x_choose(['Stop';'Go on'],'Choose');
	if flag==2 then
		x0 = evstr(x_mdialog(['Initial conditions'],ystr,string(x(:,n2))));
		x  = ode(x0,tmin,times,'wheel');
		clf(my_handle,"reset");
		show(x);
	end
end


// Clear variable and functions
// =========================================================================

clear wheelg wheelgf tmin tmax nn times x0 x ystr flag;
clear show wheeld test_wheel wheel_build_and_load get_wheel_rti;
