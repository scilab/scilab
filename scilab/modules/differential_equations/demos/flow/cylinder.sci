//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// Part II, Slanted Cylinder
// =============================================================================

function Yd=traj(tt,Y,surface)
	[p,q,r,s,t] = surface(Y(1),Y(3))
	w           = -(r*Y(2)^2+2*s*Y(2)*Y(4)+t*Y(4)^2+9.81)/(1+p^2+q^2)
	Yd          = [Y(2) ; p*w ; Y(4) ; q*w ];
endfunction


function [p,q,r,s,t]=SlantedCylinder(x,y)
	
	global g_pente;
	
	a = -tan(g_pente/180*%pi);
	z = sqrt(1-x^2)
	p = x/z;
	q = a //pente
	r = 1/(z^3)
	s = 0
	t = 0
	
endfunction

// draw_cylinder
// =============================================================================

function draw_cylinder(orig)
	
	global g_pente;
	
	my_figure_handle = scf(100001);
	
	a  = -tan(g_pente/180*%pi);
	xx = -1:0.1:1;
	yy = (0:1:14)';
	
	if orig then
		my_axe             = newaxes();
		my_axe.axes_bounds = [0,0,1,3/4];
		Z                  = -sqrt(1-ones(yy)*(xx^2))+a*yy*ones(xx);
		plot3d(xx,yy,Z');
	else
		my_figure_handle.immediate_drawing = "off";
		Z                                  = -sqrt(1-ones(yy)*(xx^2))+a*yy*ones(xx);
		my_axe                             = gca();
		e                                  = my_axe.children(size(my_axe.children,"*"));
		e.data.z                           = Z';
		my_axe.tight_limits                = "on";
		my_axe.data_bounds(1,3)            = min(Z);
		my_figure_handle.immediate_drawing = "on";
	end
	
endfunction


// draw_point
// =============================================================================

function draw_point(orig)
	
	global g_pente;
	
	global g_x;
	global g_y;
	
	my_figure_handle = scf(100001);
	
	x = g_x;
	y = g_y;
	a = -tan(g_pente*%pi/180);
	r = 0.1;
	z = -sqrt(1-x^2)+a*y+r;
	
	my_figure_handle.immediate_drawing = "off";
	
	if orig then
		param3d([x-r,x+r],[y,y],[z,z]);  e = gce(); e.thickness = 1; e.foreground = 5;
		param3d([x,x],[y-r,y+r],[z,z]);  e = gce(); e.thickness = 1; e.foreground = 5;
		param3d([x,x],[y,y],[z-r,z+r]);  e = gce(); e.thickness = 1; e.foreground = 5;
	end
	
	curAxe = gca();
	e1 = curAxe.children(1); e1.data = [x,y,z-r;x,y,z+r];
	e2 = curAxe.children(2); e2.data = [x,y-r,z;x,y+r,z];
	e3 = curAxe.children(3); e3.data = [x-r,y,z;x+r,y,z];
	
	my_figure_handle.immediate_drawing = "on";
	
endfunction

// draw_speed
// =============================================================================

function draw_speed(orig)
	
	global g_pente;
	
	global g_x;
	global g_y;
	
	global g_V;
	global g_Vdir;
	
	global speed_handle;
	
	x   = g_x;
	y   = g_y;
	
	dx1 = g_V*cos(g_Vdir*%pi/180);
	dy1 = g_V*sin(g_Vdir*%pi/180);
	
	my_figure_handle = scf(100001);
	my_figure_handle.immediate_drawing = "off";
	
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
		
	else
		curAxe                  = gca();
		curAxe.data_bounds(1,1) = -1;
		curAxe.data_bounds(1,2) =  0;
		curAxe.data_bounds(2,1) =  1;
		curAxe.data_bounds(2,3) =  0;
		speed_handle.data                  = [x,y,0;x+dx1,y+dy1,0];
		
	end
	
	my_figure_handle.immediate_drawing = "on";
	
endfunction

// calculate_traj
// =============================================================================

function Y = calculate_traj(g_x,g_y,g_V,g_Vdir)
	
	global g_pente;
	global g_x;
	global g_y;
	global g_V;
	global g_Vdir;
	
	Y0(1) = g_x;//x
	Y0(2) = g_V*cos(g_Vdir*%pi/180);//v_x
	Y0(3) = g_y;//y
	Y0(4) = g_V*sin(g_Vdir*%pi/180);//v_y
	t     = 0:0.01:10; //1001 track points
	Y     = ode(Y0,t(1),t,1d-10,1.D-10,list(traj,SlantedCylinder));//traj d'ecoulement
	
endfunction

// draw_bille
// =============================================================================

function draw_bille(Y)
	g_tx = Y(1,:)';
	g_ty = Y(3,:)';
	g_tz = -sqrt(1-g_tx^2-g_ty^2);
endfunction

// execylinder
// =============================================================================

function execylinder()

	global g_pente;
	global g_x;
	global g_y;
	global g_V;
	global g_Vdir;
	global go_on;
	global eff;
	global g_alpha;
	global g_theta;
	
	g_pente  = 14;
	
	g_x      = 0.6;
	g_y      = 0.7;
	
	g_V      = 1;
	g_Vdir   = 180;
	
	g_alpha  = 35;
	g_theta  = 45;
	
	go_on = %T; eff = %F;
	
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
	my_figure_handle.figure_name     = gettext("Cylindre");
	
	// Remove Scilab graphics menus & toolbar
	delmenu(my_figure_handle.figure_id, gettext("&File"));
	delmenu(my_figure_handle.figure_id, gettext("&Tools"));
	delmenu(my_figure_handle.figure_id, gettext("&Edit"));
	delmenu(my_figure_handle.figure_id, gettext("&?"));
	toolbar(my_figure_handle.figure_id, "off");
	
	draw_cylinder(%T);
	[g_pente,go_on]=pente(); // set the pente
	draw_point(%T);
	
endfunction
