//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
//
// This file is distributed under the same license as the Scilab package.
//


// =============================================================================
// Part I, sphere
// =============================================================================


// traj
// =============================================================================

function Yd=traj(tt,Y,surface)
  [p,q,r,s,t]=surface(Y(1),Y(3))
  w=-(r*Y(2)^2+2*s*Y(2)*Y(4)+t*Y(4)^2+9.81)/(1+p^2+q^2)
  Yd=[Y(2)
      p*w
      Y(4)
      q*w]
endfunction


// sphere
// =============================================================================

function  [p,q,r,s,t]=sphere(x,y)
	
	z = -sqrt(1-(x^2+y^2));
	w = z^3
	p = -x/z;
	q = -y/z;
	r = (y^2-1)/w;
	s = (-x*y)/w;
	t = (x^2-1)/w;
	
endfunction

// sph
// =============================================================================

function [x,y,z]=sph(alp,tet)
	x = r*cos(alp).*cos(tet)+orig(1)*ones(tet)
	y = r*cos(alp).*sin(tet)+orig(2)*ones(tet)
	z = r*sin(alp)+orig(3)*ones(tet);
endfunction


// draw_sphere
// =============================================================================

function draw_sphere()
	
	global plot_handle;
	
	my_figure_handle = scf(100001);
	r    = 1;
	orig = [0 0 0];
	[xx,yy,zz]=eval3dp(sph,linspace(-%pi/2,0,40),linspace(0,%pi*2,20));
	
	plot_handle             = newaxes();
	plot_handle.axes_bounds = [0,0,1,3/4];
	
	plot3d(xx,yy,zz);
	
endfunction

// draw_point
// =============================================================================

function draw_point(g_r,g_t,orig)
	
	x = g_r*cos(g_t*%pi/180);
	y = g_r*sin(g_t*%pi/180);
	r = 0.05;
	z = -sqrt(1-x^2-y^2)+r;
	
	my_figure_handle = scf(100001);
	my_figure_handle.immediate_drawing = "off";
	
	if orig then
		param3d([x-r,x+r],[y,y],[z,z]);  e = gce(); e.thickness = 1; e.foreground = 5;
		param3d([x,x],[y-r,y+r],[z,z]);  e = gce(); e.thickness = 1; e.foreground = 5;
		param3d([x,x],[y,y],[z-r,z+r]);  e = gce(); e.thickness = 1; e.foreground = 5;
	else
		curAxe = gca();
		e1 = curAxe.children(1); e1.data = [x,y,z-r;x,y,z+r];
		e2 = curAxe.children(2); e2.data = [x,y-r,z;x,y+r,z];
		e3 = curAxe.children(3); e3.data = [x-r,y,z;x+r,y,z];
	end
	
	my_figure_handle.immediate_drawing = "on";
	
endfunction

// draw_speed
// =============================================================================

function draw_speed(g_r,g_t,g_V,g_Vdir,orig)
	
	x = g_r*cos(g_t*%pi/180);
	y = g_r*sin(g_t*%pi/180);
	dx1 = g_V*cos(g_Vdir*%pi/180);
	dy1 = g_V*sin(g_Vdir*%pi/180);
	
	my_figure_handle = scf(100001);
	my_figure_handle.immediate_drawing = "off";
	
	global speed_handle;
	
	if orig then
		
		param3d([x,x+dx1],[y,y+dy1],[0,0]);
		
		speed_handle                       =  gce();
		speed_handle.thickness             =  1;
		speed_handle.foreground            =  5;
		speed_handle.polyline_style        =  4;
		speed_handle.arrow_size_factor     =  2;
		
		curAxe                  = gca();
		curAxe.data_bounds(1,1) = -1;
		curAxe.data_bounds(1,2) =  0;
		curAxe.data_bounds(2,1) =  1;
		curAxe.data_bounds(2,3) =  0;
		
		curAxe.data_bounds = [-1 -1 -1; 1 1 0];
	
	else
		curAxe             = gca();
		curAxe.data_bounds = [-1 -1 -1; 1 1 0];
		speed_handle.data  = [x,y,0;x+dx1,y+dy1,0];
	end
	
	my_figure_handle.immediate_drawing = "on";
	
endfunction

// calculate_traj
// =============================================================================

function Y = calculate_traj(g_r,g_t,g_V,g_Vdir)
	
	Y0(1) = g_r*cos(g_t*%pi/180);     // x
	Y0(2) = g_V*cos(g_Vdir*%pi/180);  // v_x
	Y0(3) = g_r*sin(g_t*%pi/180);     // y
	Y0(4) = g_V*sin(g_Vdir*%pi/180);  // v_z
	t     = 0:0.01:15; //1501 track points
	Y     = ode(Y0,t(1),t,1d-10,1.D-10,list(traj,sphere));//traj d'ecoulement
endfunction



function exesphere()
	
	global g_r;
	global g_t;

	global g_V;
	global g_Vdir;
	
	global go_on;
	global eff;
	
	global g_alpha;
	global g_theta;
	
	// initial point
	g_r = 0.7;
	g_t = 250;
	
	//initial speed
	g_V    = 1;
	g_Vdir = 180;
	
	//observation point
	g_alpha = 35;
	g_theta = 45;
	
	// ???
	go_on = %T;
	eff   = %F;
	
	// Figure parameters
	// =========================================================================
	
	surface_frame_w = 600;     // surface frame width
	surface_frame_h = 600;     // surface frame height
	
	control_frame_w = 600;
	control_frame_h = 200;
	
	margin_x     = 15;      // Horizontal margin between each elements
	margin_y     = 15;      // Vertical margin between each elements
	
	axes_figure_w = 2*margin_x + max(surface_frame_w,control_frame_w);
	axes_figure_h = 3*margin_y + surface_frame_h + control_frame_h;
	
	// Figure creation
	// =========================================================================
	
	my_figure_handle = scf(100001);
	clf(my_figure_handle,"reset");
	
	my_figure_handle.background      = -2;
	my_figure_handle.figure_position = [0 0];
	my_figure_handle.axes_size       = [axes_figure_w axes_figure_h];
	my_figure_handle.figure_name     = gettext("Sphere");
	
	// Remove Scilab graphics menus & toolbar
	delmenu(my_figure_handle.figure_id, gettext("&File"));
	delmenu(my_figure_handle.figure_id, gettext("&Tools"));
	delmenu(my_figure_handle.figure_id, gettext("&Edit"));
	delmenu(my_figure_handle.figure_id, gettext("&?"));
	toolbar(my_figure_handle.figure_id, "off");
	
	draw_sphere();
	draw_point(g_r,g_t,%T);
	
	// set the initial point
	[g_r,g_t]=position();

//     clearglobal g_r g_t;
//     clearglobal g_V g_Vdir;
//     clearglobal go_on eff;
//     clearglobal g_alpha g_theta;
endfunction

