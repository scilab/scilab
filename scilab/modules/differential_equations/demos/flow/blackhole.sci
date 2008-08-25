//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// Part III, black hole,z=f(x,y)=-(1/sqrt(x^2+y^2))
// =============================================================================



// traj
// ====================================

function Yd=traj(tt,Y,surface)
	[p,q,r,s,t] = surface(Y(1),Y(3));
	w           = -(r*Y(2)^2+2*s*Y(2)*Y(4)+t*Y(4)^2+9.81)/(1+p^2+q^2);
	Yd          = [ Y(2) ; p*w ; Y(4) ; q*w ];
endfunction


// blackhole
// ====================================

function  [p,q,r,s,t]=blackhole(x,y)
	
	//z=f(x,y)=-(1/sqrt(x^2+y^2));
	//
	//p=df/dx
	//q=df/dy
	//r=df/dxx
	//t=df/dyy
	//s=df/dxy
	
	z = -(1/sqrt(x^2+y^2));
	p = -x*z^3;
	q = -y*z^3;
	r = z^3*(3*x^2*z^2-1);
	s = 3*x*y*z^5;
	t = z^3*(3*y^2*z^2-1);
	
endfunction


// draw_surface
// ====================================

function draw_surface()
	
	r    = 1;
	orig = [0 0 0];
	xx   = -1.85:0.1:1.85;
	yy   = (-1.85:0.1:1.85)';
	zt   = sqrt((ones(yy)*(xx^2)+(yy^2)*ones(xx)));
	zz   = -1*ones(zt)./zt;
	
	my_handle          = scf(100001);
	
	my_axe             = newaxes();
	my_axe.axes_bounds = [0,0,1,3/4];
	
	my_handle.immediate_drawing = "off";
	my_handle.figure_position   = [350 50];
	toolbar(my_handle.figure_id, "off");
	
	mesh(xx,yy,zz);
	e            = gce();
	e.color_mode = 0;
	
	my_handle.immediate_drawing = "on";
	
endfunction

// draw_point
// ====================================

function draw_point(g_r,g_t,orig)
	
	x = g_r*cos(g_t*%pi/180);
	y = g_r*sin(g_t*%pi/180);
	r = 0.05;
	z = -(ones(x)./sqrt(x^2+y^2))+r;
	
	my_handle = scf(100001);
	my_handle.immediate_drawing = "off";
	
	if orig then
		param3d([x-r,x+r],[y,y],[z,z]);  e = gce(); e.thickness = 1; e.foreground = 5;
		param3d([x,x],[y-r,y+r],[z,z]);  e = gce(); e.thickness = 1; e.foreground = 5;
		param3d([x,x],[y,y],[z-r,z+r]);  e = gce(); e.thickness = 1; e.foreground = 5;
	else
		curAxe = gca();
		e1     = curAxe.children(1); e1.data = [x,y,z-r;x,y,z+r];
		e2     = curAxe.children(2); e2.data = [x,y-r,z;x,y+r,z];
		e3     = curAxe.children(3); e3.data = [x-r,y,z;x+r,y,z];
	end
	
	my_handle.immediate_drawing = "on";
	
endfunction

// draw_speed
// ====================================

function draw_speed(g_r,g_t,g_V,g_Vdir,orig)
	
	x   = g_r*cos(g_t*%pi/180);
	y   = g_r*sin(g_t*%pi/180);
	dx1 = g_V*cos(g_Vdir*%pi/180)/2;
	dy1 = g_V*sin(g_Vdir*%pi/180)/2;
	
	my_handle = scf(100001);
	my_handle.immediate_drawing = "off";
	
	if orig then
		param3d([x,x+dx1],[y,y+dy1],[0,0]);
		e                   = gce();
		e.thickness         = 1;
		e.foreground        = 5;
		e.polyline_style    = 4;
		e.arrow_size_factor = 2;
		curAxe              = gca();
		curAxe.data_bounds  = [-2 -2 -16; 2 2 0];
	else
		curAxe              = gca();
		curAxe.data_bounds  = [-2 -2 -16; 2 2 0];
		e                   = curAxe.children(1);
		e.data              = [x,y,0;x+dx1,y+dy1,0];
	end
	
	my_handle.immediate_drawing = "on";
	
endfunction


// clear_ini
// ====================================

function clear_ini()
	
	curAxe = gca();
	num    = size(curAxe.children,'*');
	
	my_handle = scf(100001);
	my_handle.immediate_drawing = "off";
	
	for i=num:-1:1
		if (curAxe.children(i).type == "Polyline")
			delete(curAxe.children(i))
		end
	end
	
	my_handle.immediate_drawing = "off";
	
endfunction



// calculate_traj
// ====================================

function Y = calculate_traj(g_r,g_t,g_V,g_Vdir)
	
	Y0(1) = g_r*cos(g_t*%pi/180);      // x
	Y0(2) = g_V*cos(g_Vdir*%pi/180);   // v_x
	Y0(3) = g_r*sin(g_t*%pi/180);      // y
	Y0(4) = g_V*sin(g_Vdir*%pi/180);   // v_y
	
	t     = 0:0.02:10;                 // 501 track points
	
	Y     = ode(Y0,t(1),t,1d-10,1.D-10,list(traj,blackhole)); //traj d'ecoulement
	
endfunction


// exeblackhole
// ====================================

function exeblackhole()
	
	//initial point
	global g_r;
	global g_t;
	g_r = 1.1;
	g_t = 41;
	
	//initial speed
	global g_V;
	global g_Vdir;
	g_V    = 1.65;
	g_Vdir = 165;
	
	// ???
	global go_on;
	global eff;
	go_on = %T;
	eff   = %F;
	
	// observation point
	global g_alpha
	global g_theta;
	g_alpha = 35;
	g_theta = 45;
	
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
	my_figure_handle.figure_name     = gettext("Blackhole");
	
	// Remove Scilab graphics menus & toolbar
	delmenu(my_figure_handle.figure_id, gettext("&File"));
	delmenu(my_figure_handle.figure_id, gettext("&Tools"));
	delmenu(my_figure_handle.figure_id, gettext("&Edit"));
	delmenu(my_figure_handle.figure_id, gettext("&?"));
	toolbar(my_figure_handle.figure_id, "off");
	
	// Actions
	draw_surface();
	draw_point(g_r,g_t,%T);
	[g_r,g_t,go_on]=position(); // set the initial point
	
endfunction
