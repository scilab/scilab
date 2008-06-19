//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

funcprot(0);

function demo_riemann()
	
	global quit_var;
	global pause_var;
	
	quit_var  = 0;
	pause_var = 0;
	
	// Figure creation
	// =========================================================================
	my_handle = scf(100001);
	clf(my_handle,"reset");
	
	// Drawlater
	// =========================================================================
	my_handle.immediate_drawing = "off";
	
	
	// Colormap
	// =========================================================================
	my_handle.color_map = rainbowcolormap(128);
	
	
	// Compute
	// =========================================================================
	[z,s]               = cplxroot(4,35);
	
	
	// Draw
	// =========================================================================
	cplxmap(z,s,163,69);  //draw
	
	
	// Adjust some graphical parameters
	// =========================================================================
	my_axe               = gca();
	my_fac3d             = gce();
	
	my_handle.axes_size  = [ 650 650 ];
	
	my_handle.background = -1;                 // black
	my_axe.background    = -1;                 // gray
	my_axe.foreground    = 14;                 // white
	my_fac3d.color_mode  =  1;                 // no lines
	
	
	// Stop => Pushbutton
	// =========================================================================
	
	my_quit_button = uicontrol( ...
		"parent"              , my_handle,...
		"style"               , "pushbutton",...
		"string"              , "STOP",...
		"units"               , "pixels",...
		"position"            , [ 200 10 40 40 ],...
		"background"          , [1 1 1], ...
		"callback"            , "quit_riemann",...
		"tag"                 , "pushbutton_bac" ...
	);
	
	my_pause_button = uicontrol( ...
		"parent"              , my_handle,...
		"style"               , "pushbutton",...
		"string"              , "PAUSE",...
		"units"               , "pixels",...
		"position"            , [ 300 10 40 40 ],...
		"background"          , [1 1 1], ...
		"callback"            , "pause_riemann",...
		"tag"                 , "my_pause_button" ...
	);
	
	my_play_button = uicontrol( ...
		"parent"              , my_handle,...
		"style"               , "pushbutton",...
		"string"              , "PLAY",...
		"units"               , "pixels",...
		"position"            , [ 300 10 40 40 ],...
		"background"          , [1 1 1], ...
		"callback"            , "play_riemann",...
		"visible"             , "off",...
		"tag"                 , "my_play_button" ...
	);
	
	// Drawnow
	// =========================================================================
	my_handle.immediate_drawing = "on";
	
	
	realtimeinit(0.01);
	
	for k=1:360
		
		realtime(k);
		
		if quit_var == 1 then
			delete(my_handle);
			return;
		end
		
		if play_var == 1 & my_play_button.visible == "on" then
			my_play_button.visible  = "off";
			my_pause_button.visible = "on";
		end
		
		if pause_var == 0 then
			if my_play_button.visible == "on" then
				my_play_button.visible   = "on";
				my_pause_button.visible  = "off";
			end
			my_axe.rotation_angles = my_axe.rotation_angles + [0 1];
		end
		
	end
	
	for k=1:360
		realtime(k);
		if stop == 0 then
			my_axe.rotation_angles = my_axe.rotation_angles + [1 0];
		end
	end
	
endfunction


function cplxmap(z,w,varargin)
	
	//cplxmap(z,w,T,A,leg,flags,ebox)
	//cplxmap Plot a function of a complex variable.
	//       cplxmap(z,f(z))
	
	x            = real(z);
	y            = imag(z);
	u            = real(w);
	v            = imag(w);
	
	my_handle    = gcf();
	my_color_map = my_handle.color_map;
	ncols        = size(my_color_map,:);
	
	[X,Y,U]      = nf3d(x,y,u);
	[X,Y,V]      = nf3d(x,y,v);
	Colors       = sum(V,'r');
	Colors       = Colors - min(Colors);
	Colors       = int((ncols-1)*Colors/max(Colors)+1);
	
	plot3d(X,Y,list(U,Colors),varargin(:));
	
endfunction



function [z,s]=cplxroot(n,m)
	
	//cplxroot(n,m,T,A,leg,flags,ebox)
	//CPLXROOT Riemann surface for the n-th root.
	//       CPLXROOT(n) renders the Riemann surface for the n-th root.
	//       CPLXROOT, by itself, renders the Riemann surface for the cube root.
	//       CPLXROOT(n,m) uses an m-by-m grid.  Default m = 20.
	// Use polar coordinates, (r,theta).
	// Cover the unit disc n times.
	// Copyright INRIA
	
	[lhs,rhs]=argn(0)
	
	if rhs  < 1, n = 3; end
	if rhs  < 2, m = 20; end
	
	r     = (0:m)'/m;
	theta = - %pi*(-n*m:n*m)/m;
	z     = r * exp(%i*theta);
	s     = r.^(1/n) * exp(%i*theta/n);
	
endfunction




function quit_riemann
	global quit_var;
	quit_var = 1;
endfunction


function pause_riemann
	global pause_var;
	pause_var = 1;
endfunction


function play_riemann
	global pause_var;
	pause_var = 0;
endfunction




funcprot(1);
